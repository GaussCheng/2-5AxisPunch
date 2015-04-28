#include "icprogrampage.h"
#include "ui_icprogrampage.h"
#include <QDebug>
#include "icparameterssave.h"
#include "icmold.h"
#include <QMessageBox>
#include "moldinformation.h"
#include "icparameterconversion.h"
#include "QtCore/qmath.h"
#include "icvirtualkey.h"
ICProgramPage * ICProgramPage::instance_;


ICProgramPage::ICProgramPage(QWidget *parent,int _pageIndex,QString pageName) :
    QWidget(parent),
    ui(new Ui::ICProgramPage),
    _index(_pageIndex),
    _pageName(pageName),
    _pageMaxCount(PAGE_MAX_COUNT),
    _currentPage(0)
{
    ui->setupUi(this);
    _dialog =  VirtualNumericKeypadDialog::Instance();
    _typeDialog =  ICPointType::Instance(this);
    _host = ICVirtualHost::GlobalVirtualHost();
    ui->modiifyButton->hide();
    ui->pushButton->hide();
    ui->seveoEdit->setCheckedName(tr("Servo Off"));
    ui->seveoEdit->setName(tr("Servo On"));

    QFile file("./sysconfig/StandPrograms");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString tmp;
        while(!(in>>tmp).atEnd())
        {
            standPrograms_.append(tmp);
        }
        file.close();
    }

    InitTableWidget();
    InitPoint();
    InitFixMoldItems();
    InitPointToItem();
    connect(MoldInformation::Instance(),SIGNAL(MoldChanged(QString)),
            SLOT(MoldChanged(QString)));

    allItems = GT_AllMoldItems();


    for(int i=0;i<AXIS_COUNTS;i++)
        validators_.append(new  QIntValidator(0,10000,this));
//    ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(3,0));

}


QList<ICMoldItem> ICProgramPage::GT_MoldItems()
{
    QList<ICMoldItem> items;
    QList<ICMoldItem> fixItems;

    for(int i=0;i < ROW_COUNTS;i++){
        items += MK_PosItem(i);
        fixItems = pointToItem.value(pointTypes.at(i));
        if(fixItems.size()){
            items += fixItems;
        }
    }

    return items;
}

QList<ICMoldItem> ICProgramPage::GT_HeaderItems()
{
    QList<ICMoldItem> items;
    return items;
}

QList<ICMoldItem> ICProgramPage::GT_TailMoldItems()
{
    QList<ICMoldItem> items;
    items << MK_MoldItem(5,5,255,32,0,0,0,0,0,42);
    return items;
}

void ICProgramPage::refreshCurrentRow(int step)
{
//    int step = _host->HostStatus(ICVirtualHost::ICStatus(ICVirtualHost::Step)).toInt();
//    if(step > allItems.size()){
//        Q_ASSERT(false);
//    }
    QMap<int,int> stepToRow;
    if(step >= allItems.size() || step < 0){
        return;
    }

    int startIndex = 0,oldIndex = 0,oldPoint=0;
    foreach(ICMoldItem item,allItems){
        if(item.GMVal() != 22){
            stepToRow.insert(item.Num(),oldIndex);
        }
        else{
            stepToRow.insert(item.Num(),startIndex);
            if(oldPoint != item.SubNum()){
                startIndex++;
            }
            oldIndex = startIndex;
            oldPoint = item.SubNum();

        }
    }

    int row = stepToRow.value(step);
    ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(row,0));

}

QTableWidget *ICProgramPage::TableWidget()
{
    return ui->tableWidget;
}

QList<ICMoldItem> ICProgramPage::MK_PosItem(int pos)
{
    QList<ICMoldItem> items;

    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY1)) ==
            ICVirtualHost::ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY1))){
        items << MK_MoldItem(1,1,pos,22,0,64,2,100,0,172);  //3D教导
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX1)) ==
            ICVirtualHost::ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX1))){
        items  << MK_MoldItem(2,1,pos,22,0,64,1,100,0,174);//3D教导
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisZ)) ==
            ICVirtualHost::ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisZ))){
        items  << MK_MoldItem(3,1,pos,22,0,64,3,100,0,176);  //3D教导
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX2)) ==
            ICVirtualHost::ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX2))){
        items  << MK_MoldItem(4,1,pos,22,0,64,4,100,0,178);  //3D教导
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY2)) ==
            ICVirtualHost::ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY2))){
        items  << MK_MoldItem(5,1,pos,22,0,64,5,100,0,180);  //3D教导
    }


    return items;
}

ICProgramPage::~ICProgramPage()
{
    delete ui;
    for(int i=0;i<AXIS_COUNTS;i++)
        delete validators_[i];
}

void ICProgramPage::ChangeDelay(int delay)
{
    //发送教导修改指令
    outY37On.SetDVal(delay);
    outY37Off.SetDVal(delay);
    InitPointToItem();
    ReConfigure();

}

void ICProgramPage::showEvent(QShowEvent *e)
{
    //隐藏列
    for(int i=0;i < AXIS_COUNTS;i++){
        if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX1 + i)) != ICVirtualHost::ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX1)))
        {
            ui->tableWidget->setColumnHidden(i + 1,true);
        }
        else{
            ui->tableWidget->setColumnHidden(i + 1,false);

        }
    }

    ui->verticalLayout->insertWidget(0,ui->tableWidget);
    ui->tableWidget->setColumnHidden(6,false);
    ui->tableWidget->setColumnHidden(7,false);
    if(USE_SPACE_ROW)
    ui->tableWidget->setRowHidden(ui->tableWidget->rowCount() - 1,false);

    for(int i=0;i<AXIS_COUNTS;i++){
        int top = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::ICSystemParameter(ICVirtualHost::SYS_X_Length + i * 7)).toInt();
        int bottom = ICVirtualHost::GlobalVirtualHost()->SystemParameter(ICVirtualHost::ICSystemParameter(ICVirtualHost::SYS_X_Maxium + i * 7)).toInt();
        validators_.at(i)->setTop(top);
        validators_.at(i)->setBottom(bottom);
    }

    QWidget::showEvent(e);
}

void ICProgramPage::hideEvent(QHideEvent *e)
{
    ui->startEdit->setChecked(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

//    SaveConfigPoint();
    ReConfigure();

    QWidget::hideEvent(e);

}

void ICProgramPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:{
        ui->retranslateUi(this);
        QStringList headerContents;
        headerContents  <<  tr("Type") << tr("FrontBack") << tr("UpDown") << tr("Windup") <<  tr("Rotated")
                        << tr("Rollovers")  << tr("Save") << tr("Tested");

        for(int i=0;i<headerContents.size();i++){
            ui->tableWidget->horizontalHeaderItem(i)->setText(headerContents.at(i));
        }
        _typeDialog->Init_();
        InitPoint();
    }
        break;
    default:
        break;
    }
}

void ICProgramPage::itemClicked(QTableWidgetItem *item)
{
    if(!ui->startEdit->isChecked())
        return;

    if((item->row() >=0 && item->row() < ROW_COUNTS)  &&
        (item->column() >0 && item->column() < AXIS_COUNTS + 1)){

        if(!_dialog->isVisible()){
            _dialog->move(200,200);
            _dialog->exec();
        }

        int column = item->column();

        QString text = _dialog->GetCurrentText();

        if(!text.isEmpty() && (text != item->text())){
            int value;
            if(text.contains(QChar('.'))){
                int pointSize = text.split(".").at(1).size();
                if(pointSize > POINT_SIZE || pointSize==0){
                    QMessageBox::information(this,tr("information"),tr("Input Error!"));
                    _dialog->ResetDisplay();
                    return;
                }
                else{
                    value = text.remove(".").toInt();
                }

            }
            else{
                value = text.toInt() * qPow(10,POINT_SIZE);
            }


            QIntValidator *v = validators_[column-1];
            qDebug() << v->bottom() << v->top();
            if(value > v->top() || value < v->bottom()){
                QString format = QString("%.%1f").arg(POINT_SIZE);
                QString bottom =   QString().sprintf(format.toAscii(), v->bottom() / static_cast<qreal>(qPow(10, POINT_SIZE)));
                QString top =   QString().sprintf(format.toAscii(), v->top() / static_cast<qreal>(qPow(10, POINT_SIZE)));

                QMessageBox::information(this,tr("information"),tr("Input Value Not %1 To %2 Range!")
                                         .arg(bottom)
                                         .arg(top));
                _dialog->ResetDisplay();
                return;
            }
            item->setText(ICParameterConversion::TransThisIntToThisText(value, POINT_SIZE));
        }

        _dialog->ResetDisplay();

    }

}

void ICProgramPage::saveButtonsCliked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int index = saveButtons.indexOf(button);

    for(int i=0; i < AXIS_COUNTS;i++){
        int16_t pos = _host->HostStatus(ICVirtualHost::ICStatus(ICVirtualHost::XPos + i)).toInt();
        ui->tableWidget->item(index,1+i)->setText(QString::number(pos / 10.0, 'f', 1));
        ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(_index * 6 * MAX_POINTS + index * 6 + i),pos);

    }
}

void ICProgramPage::testButonsPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int index = testButtons.indexOf(button);

    if(index < 0)
    {
        return;
    }
    if(!ICVirtualHost::GlobalVirtualHost()->IsOrigined())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Has not been origin!"));
        return;
    }
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GARC);
    cmd.SetPoint(index);
    //    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
}


void ICProgramPage::on_pushButton_clicked()
{
    emit returnProgram();
}

void ICProgramPage::InitTableWidget()
{
//    ui->tableWidget->setRowCount(MAX_POINTS);
    ui->tableWidget->setColumnCount(COLUMN_COUNTS);


    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
//    ui->tableWidget->verticalHeader()->hide();

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList headerContents;
    headerContents  <<  tr("Type") << tr("FrontBack") << tr("UpDown") << tr("Windup") <<  tr("Rotated")
                    << tr("Rollovers")  << tr("Save") << tr("Tested");

    for(int i=0;i<headerContents.size();i++){
        ui->tableWidget->setHorizontalHeaderItem(i,new QTableWidgetItem(headerContents[i]));
    }

    if(USE_SPACE_ROW)
        ui->tableWidget->insertRow(0);
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(itemClicked(QTableWidgetItem*)));
}

void ICProgramPage::InitPoint()
{
    DeleteWidgets();
    allPoints.clear();

    uint pointCount = _NativeMoldParam(ICMold::pointCount);
    quint64 pointConfig = (quint32)_NativeMoldParam(ICMold::pointConfig1)  |
                          ((quint64)(_NativeMoldParam(ICMold::pointConfig2)) << 32);
//    quint64 pointConfig2 = _NativeMoldParam(ICMold::pointConfig3)  |
//                              ((quint64)(_NativeMoldParam(ICMold::pointConfig4)) << 32);


    for(int i =0 ;i < pointCount;i++){
        if(i < 16){
            pointTypes.append((PointType)(pointConfig >> (i * 4) & 0xF));
        }
//        else{
//            pointTypes.append((PointType)(pointConfig2 >> ((i- 16) * 4) & 0xF));
//        }
    }

    for(int i = 0 ;i < pointCount;i++){
        ui->tableWidget->insertRow(ROW_COUNTS);

        QPushButton *button = new QPushButton(this);
        saveButtons.append(button);
        saveButtons[i]->setText(tr("save"));
        saveButtons[i]->setMinimumHeight(40);
        saveButtons[i]->setFocusPolicy(Qt::NoFocus);
        connect(saveButtons[i],SIGNAL(clicked()),
                SLOT(saveButtonsCliked()));

        button = new QPushButton(this);
        testButtons.append(button);
        testButtons[i]->setText(tr("test"));
        testButtons[i]->setMinimumHeight(40);
        testButtons[i]->setFocusPolicy(Qt::NoFocus);
        connect(testButtons[i],SIGNAL(pressed()),
                SLOT(testButonsPressed()));
        connect(testButtons[i],SIGNAL(released()),
                SLOT(testButonsReleased()));

        allPoints.append(MK_Point(_MoldParam(ICMold::point0 + i * 6 + 0 + _index * 6 * MAX_POINTS),
                                  _MoldParam(ICMold::point0 + i * 6 + 1 + _index * 6 * MAX_POINTS),
                                  _MoldParam(ICMold::point0 + i * 6 + 2 + _index * 6 * MAX_POINTS),
                                  _MoldParam(ICMold::point0 + i * 6 + 3 + _index * 6 * MAX_POINTS),
                                  _MoldParam(ICMold::point0 + i * 6 + 4 + _index * 6 * MAX_POINTS)
                                  ));

        ui->tableWidget->setCellWidget(i,AXIS_COUNTS + 1,saveButtons[i]);
        ui->tableWidget->setCellWidget(i,AXIS_COUNTS + 2,testButtons[i]);
    }
    for(int k=0;k< ui->tableWidget->verticalHeader()->count();k++){
        ui->tableWidget->verticalHeader()->resizeSection(k,40);
    }


    for(int j =0; j< pointCount;j++){
        for(int i=0;i <COLUMN_COUNTS ; i++){
            if(!ui->tableWidget->cellWidget(j,i) && !ui->tableWidget->item(j,i)){
                QTableWidgetItem * item = new QTableWidgetItem();
                item->setText("");
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->tableWidget->setItem(j,i,item);
            }
        }
        ui->tableWidget->item(j,0)->setText(_typeDialog->toString(pointTypes.at(j)));
    }


    for(int i=0;i<pointCount;i++){
        ui->tableWidget->item(i,1)->setText(QString::number(allPoints[i]->x / 10.0, 'f', 1));
        ui->tableWidget->item(i,2)->setText(QString::number(allPoints[i]->y / 10.0, 'f', 1));
        ui->tableWidget->item(i,3)->setText(QString::number(allPoints[i]->s / 10.0, 'f', 1));
        ui->tableWidget->item(i,4)->setText(QString::number(allPoints[i]->r / 10.0, 'f', 1));
        ui->tableWidget->item(i,5)->setText(QString::number(allPoints[i]->t / 10.0, 'f', 1));
    }
}

void ICProgramPage::DisableTestButtons()
{
    foreach(QPushButton *button,testButtons){
        button->setEnabled(false);
    }
}

void ICProgramPage::EnableTestButtons()
{
    foreach(QPushButton *button,testButtons){
        button->setEnabled(true);
    }
}

void ICProgramPage::DeleteWidgets()
{
    int rowCount = ROW_COUNTS;
    for(int index=0;index < rowCount;index++){
        for(int i=0;i < ui->tableWidget->columnCount();i++){
            if(ui->tableWidget->item(index,i))
            delete ui->tableWidget->item(index,i);
        }
        delete saveButtons[0];
        delete testButtons[0];

        saveButtons.removeAt(0);
        testButtons.removeAt(0);
        ui->tableWidget->removeRow(0);
        pointTypes.removeAt(0);
    }
//    for(int k=0;k< ui->tableWidget->verticalHeader()->count();k++){
//        ui->tableWidget->verticalHeader()->resizeSection(k,40);
//    }

}

void ICProgramPage::InitPointToItem()
{
    QList<ICMoldItem> items;
    pointToItem.clear();
    pointToItem.insert(Get_Wait,(items << waitM10)); items.clear();
    pointToItem.insert(Get_Up,items); items.clear();
    pointToItem.insert(Get,(items << outY37On)); items.clear();
    pointToItem.insert(Put_Wait,(items << outM27On << outM11 << waitM12)); items.clear();
//    pointToItem.insert(Put_Wait,items); items.clear();
    pointToItem.insert(Put_Up,items); items.clear();
    pointToItem.insert(Put,(items << outY37Off)); items.clear();
    pointToItem.insert(Put_Wait2,(items  << waitM14 << outPermit)); items.clear();
    pointToItem.insert(Reserve,items); items.clear();


}

void ICProgramPage::SaveConfigPoint()
{
    //保存点配置
    quint64 config1 = 0;
//    quint64 config2 = 0;

    for(int i=0;i<pointTypes.size();i++){
        if(i < 16){
            quint64 type =  pointTypes.at(i);
            config1 |= (type  << (i *4));
        }
//        else{
//            quint64 type =  pointTypes.at(i);
//            config2 |= (type  << (i-16) * 4);
//        }
    }

    bool saved = false;
    if(_NativeMoldParam(ICMold::pointCount) != ROW_COUNTS){
        _SetNativeMoldParam(ICMold::pointCount, ROW_COUNTS);
        saved = true;
    }

    if(_NativeMoldParam(ICMold::pointConfig1) != config1 & 0xFFFFFFFF){
        _SetNativeMoldParam(ICMold::pointConfig1,config1 & 0xFFFFFFFF);
        saved = true;
    }

    if(_NativeMoldParam(ICMold::pointConfig2) != (config1 >> 32) & 0xFFFFFFFF){
        _SetNativeMoldParam(ICMold::pointConfig2,(config1 >> 32) & 0xFFFFFFFF);
        saved = true;
    }

//    if(_NativeMoldParam(ICMold::pointConfig3) != config2 & 0xFFFFFFFF){
//        _SetNativeMoldParam(ICMold::pointConfig3,config2 & 0xFFFFFFFF);
//        saved = true;

//    }
//    if(_NativeMoldParam(ICMold::pointConfig4) != (config2 >> 32) & 0xFFFFFFFF){
//        _SetNativeMoldParam(ICMold::pointConfig4,(config2 >> 32) & 0xFFFFFFFF);
//        saved = true;
//    }

    if(saved){
        ICMold::CurrentMold()->SaveMoldConfigFile();
    }


    saved = false;
    //保存点坐标
    for(int i=0;i<ROW_COUNTS;i++){
        for(int j=0;j<AXIS_COUNTS;j++){
            int16_t pos = ui->tableWidget->item(i,j+1)->text().remove(".").toInt();
            int16_t oldPos = _MoldParam(_index * 6 * MAX_POINTS + i * 6 + j);
            if(pos != oldPos){
                saved = true;
                _SetMoldParam(_index * 6 * MAX_POINTS + i * 6 + j,pos);
            }
        }
    }
    if(saved){
        ICMold::CurrentMold()->SaveMoldParamsFile();
    }
}

QList<ICMoldItem> ICProgramPage::GT_AllMoldItems()
{
    QList<ICMoldItem> items;
    items =    GT_HeaderItems()
            +  GT_MoldItems()
            +  GT_TailMoldItems();
    GT_CalculateItem(items);
    ICMold::MoldReSum(items);
    allItems = items;
    return items;
}



PointPtr ICProgramPage::MK_Point(qint16 x, qint16 y, qint16 s, qint16 r, qint16 t)
{
    PointPtr p(new ICPoint_);
    p->x = x;
    p->y = y;
    p->s = s;
    p->r = r;
    p->t = t;

    return p;
}

ICMoldItem ICProgramPage::MK_MoldItem(uint seq, uint num, uint8_t subNum, uint gmVal, uint pos, uint ifVal, uint ifPos, uint sVal, uint dVal, uint sum)
{
    ICMoldItem p;
    p.SetSeq(seq);
    p.SetNum(num);
    p.SetSubNum(subNum);
    p.SetGMVal(gmVal);
    p.SetPos(pos);
    p.SetIFVal(ifVal);
    p.SetIFPos(ifPos);
    p.SetSVal(sVal);
    p.SetDVal(dVal);


    return p;

}

void ICProgramPage::InitFixMoldItems()
{
    waitM10   = MK_MoldItem(6,2,0,24,0,1,0,0,3000,33);
    outY37On  = MK_MoldItem(7,3,23,12,0,1,0,0,0,45);
    outM11    = MK_MoldItem(8,4,1,25,0,1,0,0,0,204);
    waitM12   = MK_MoldItem(9,5,2,24,0,1,0,0,3000,207);
    outY37Off = MK_MoldItem(10,6,23,12,0,0,0,0,0,50);
    outY31On  = MK_MoldItem(11,7,17,11,0,1,0,0,0,47);
    outY31Off = MK_MoldItem(12,8,17,11,0,0,0,0,0,98);
    outM27On  = MK_MoldItem(13,9,15,25,0,1,0,0,0,246); //新添加
    waitM14   = MK_MoldItem(13,9,4,24,0,1,0,0,3000,246);
    outPermit = MK_MoldItem(14,10,0,27,0,1,0,0,0,103);

    outY37On.SetDVal(_NativeMoldParam(ICMold::ClipDelay));
    outY37Off.SetDVal(_NativeMoldParam(ICMold::ClipDelay));

}

void ICProgramPage::SetPageMaxCount(int count)
{
    _pageMaxCount = count;
}


void ICProgramPage::on_newButton_clicked()
{
    int index = ui->tableWidget->currentIndex().row();
    if(index == -1) index ++;
    QString moldName = ICMold::CurrentMold()->MoldName().split("/")[2];
    if(standPrograms_.contains(moldName)){
        QMessageBox::information(this,tr("Information"),tr("Standard Mold Cannot Modify!"));
        return;
    }

    if(ui->tableWidget->rowCount() == MAX_ROWCOUNT){
        QMessageBox::information(this,tr("Information"),tr("Max Rows Beyond %1!").arg(MAX_ROWCOUNT));
        return;
    }

#ifndef Q_WS_QWS
    if(_typeDialog->exec() == QDialog::Accepted){
        ui->tableWidget->insertRow(index);
        ui->tableWidget->setItem(index,0,new QTableWidgetItem(_typeDialog->toString()));
#else
      ui->tableWidget->insertRow(index);
      ui->tableWidget->setItem(index,0,new QTableWidgetItem(_typeDialog->toString(Reserve)));

#endif

        ui->tableWidget->item(index,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        for(int i = 0 ;i < AXIS_COUNTS; i++){
            QTableWidgetItem *item = new QTableWidgetItem("") ;
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setText("0.0");
            ui->tableWidget->setItem(index,i+1,item);
        }
        QPushButton *button = new QPushButton;
        saveButtons.insert(index,button);
        saveButtons[index]->setText(tr("save"));
        saveButtons[index]->setMinimumHeight(40);
        saveButtons[index]->setFocusPolicy(Qt::NoFocus);
        connect(saveButtons[index],SIGNAL(clicked()),
                SLOT(saveButtonsCliked()));
        ui->tableWidget->setCellWidget(index,AXIS_COUNTS + 1,saveButtons[index]);

        button = new QPushButton;
        testButtons.insert(index,button);
        testButtons[index]->setText(tr("test"));
        testButtons[index]->setMinimumHeight(40);
        testButtons[index]->setFocusPolicy(Qt::NoFocus);
        connect(testButtons[index],SIGNAL(pressed()),
                SLOT(testButonsPressed()));
        connect(testButtons[index],SIGNAL(released()),
                SLOT(testButonsReleased()));
        ui->tableWidget->setCellWidget(index,AXIS_COUNTS + 2,testButtons[index]);
#ifndef Q_WS_QWS
        pointTypes.insert(index,_typeDialog->currentType());
#else
        pointTypes.insert(index,Reserve);
#endif
        for(int k=0;k< ui->tableWidget->verticalHeader()->count();k++){
            ui->tableWidget->verticalHeader()->resizeSection(k,40);
        }
        DisableTestButtons();

#ifndef Q_WS_QWS
    }
#endif

}

void ICProgramPage::on_modiifyButton_clicked()
{
    int index = ui->tableWidget->currentIndex().row();
    if(index == -1) index ++;
    if(_typeDialog->exec() == QDialog::Accepted){
        pointTypes[index] = _typeDialog->currentType();
        ui->tableWidget->item(index,0)->setText(_typeDialog->toString());
    }
}

void ICProgramPage::on_deleteButton_clicked()
{

    int index = ui->tableWidget->currentIndex().row();
    if(index == -1) index ++;

    QString moldName = ICMold::CurrentMold()->MoldName().split("/")[2];
    if(standPrograms_.contains(moldName)){
        QMessageBox::information(this,tr("Information"),tr("Standard Mold Cannot Modify!"));
        return;
    }

#ifdef Q_WS_QWS
    if(pointTypes.at(index) == Get_Wait ||
        pointTypes.at(index)== Get ||
        pointTypes.at(index)== Get_Up ||
        pointTypes.at(index)== Get_Wait2 ||
        pointTypes.at(index) == Put_Wait ||
        pointTypes.at(index) == Put ||
        pointTypes.at(index) == Put_Up ||
        pointTypes.at(index) == Put_Wait2 ){
        QMessageBox::information(this,tr("Information"),tr("Canot Delete %1 action!").arg( _typeDialog->toString(pointTypes.at(index) )));
        return;
    }
#endif

    if(QMessageBox::information(this,tr("Information"),tr("If Delete current Row?"),
                                QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok)
    {
        return;
    }

    if(index != ROW_COUNTS){
        for(int i=0;i < ui->tableWidget->columnCount();i++){
            delete ui->tableWidget->item(index,i);
        }
        delete saveButtons[index];
        delete testButtons[index];

        saveButtons.removeAt(index);
        testButtons.removeAt(index);
        ui->tableWidget->removeRow(index);
        pointTypes.removeAt(index);

    }
    else{
        QMessageBox::information(this,tr("Information"),tr("Canot Delete Space Row!"));
        return;
    }
    ui->tableWidget->hide();
    ui->tableWidget->show();
    DisableTestButtons();

}

void ICProgramPage::on_saveButton_clicked()
{
    SaveConfigPoint();
    ReConfigure();
    EnableTestButtons();
}

void ICProgramPage::MoldChanged(QString s)
{
    InitPoint();
    allItems = GT_AllMoldItems();

}


void ICProgramPage::GT_CalculateItem(QList<ICMoldItem>& items)
{
    uint oldNum = 0;
    //计算num
    for(int i=0;i<items.size();i++){
        if(items.at(i).GMVal() == ICMold::GARC){
            items[i].SetNum(oldNum);
            if(items.at(i).IFPos()  == 5){
                oldNum ++;
            }
        }
        else{
            items[i].SetNum(oldNum);
            oldNum ++;
        }

    }
}


bool ICProgramPage::MoldChanged(QList<ICMoldItem>& items)
{
    QList<ICMoldItem> oldItems = ICMold::CurrentMold()->MoldContent();
    if(items.size() != oldItems.size()){
        return true;
    }
    for(int i=0;i<items.size();i++){
        if(!(items.at(i) == oldItems.at(i))){
            return true;
        }
    }

    return false;
}

bool ICProgramPage::MoldConfigChanged()
{
    return true;
}



void ICProgramPage::on_startEdit_clicked(bool checked)
{
    if(checked){
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    }
    else{
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

void ICProgramPage::ReConfigure()
{
    allItems = GT_AllMoldItems();
    if(MoldChanged(allItems)){
        ICMold::CurrentMold()->SetMoldContent(allItems);
        qDebug() << "SaveProgramToFiles: " << ICMold::CurrentMold()->SaveMoldFile(false);
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();

    }
}



void ICProgramPage::on_seveoEdit_toggled(bool checked)
{
    if(checked)
    {
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_SERVO_OFF);
    }
    else
    {
        ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_SERVO_ON);
    }
}

void ICProgramPage::testButonsReleased()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_TESTSTOP);

}
