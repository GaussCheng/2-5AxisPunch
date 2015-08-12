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
#include "icsystemconfig.h"
ICProgramPage * ICProgramPage::instance_;


quint32 _BUILD_INT32(quint32 low,quint32 high){
        quint32 temp;
        temp = ( high << 16 )  |  low;
        return temp;
}


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
    _itemDialog = new ICWidgetItemKeyboard();
    _typeDialog =  ICPointType::Instance(this);
    _host = ICVirtualHost::GlobalVirtualHost();
    ui->modiifyButton->hide();
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

    connect(ui->injectButton,SIGNAL(clicked()),
            SLOT(saveButtonsCliked()));
    connect(ui->testButton,SIGNAL(pressed()),
            SLOT(testButonsPressed()));
    connect(ui->testButton,SIGNAL(released()),
            SLOT(testButonsReleased()));

    allItems = GT_AllMoldItems();


    for(int i=0;i<AXIS_COUNTS;i++)
        validators_.append(new  QIntValidator(0,10000,this));

    dValidator = new QIntValidator(0,MAX_DEALY,this);

    oriStyle = ui->manualButton->styleSheet();
    yOnStyle = "border-style: outset;"
            "border-width: 2px;"
            "border-radius: 6px;"
            "border-color: gray;"
            "background-color: green;"
            "padding-right: 6px;"
            "padding-left:6px;";
//    ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(3,0));
    manualButtons << ui->manualButton << ui->manual2Button;
    for(int i=0;i < manualButtons.size();i++){
        connect(manualButtons.at(i),SIGNAL(clicked()),
                this,SLOT(OnShortcutTriggered()));
        connect(manualButtons.at(i),SIGNAL(clicked()),
                this,SLOT(OnShortcutReleased()));    }

//       ui->saveButton->hide();
}


QList<ICMoldItem> ICProgramPage::GT_MoldItems()
{
    QList<ICMoldItem> items;
    QList<ICMoldItem> fixItems;
    QList<ICMoldItem> posItems;

    ICMoldItem propertyItems;


    for(int i=0;i < ROW_COUNTS;i++){
        if(pointConfigs[i].Type() != Point_Property){
            if(pointConfigs[i].Type() == Wait_Safe){
                fixItems = pointToItem.value((PointType)pointConfigs[i].Type());
                if(fixItems.size()){
                    items += fixItems;
                }
                posItems = MK_PosItem(i,GT_PointIndexFromRow(i));
                for(int t =0;t < posItems.size();t++){
                    posItems[t].SetDVal(pointConfigs[i].Delay());
                    posItems[t].SetSVal(pointConfigs[i].Speed());
                }
                items += posItems;
            }
            else{
                posItems = MK_PosItem(i,GT_PointIndexFromRow(i));
                for(int t =0;t < posItems.size();t++){
                    posItems[t].SetDVal(pointConfigs[i].Delay());
                    posItems[t].SetSVal(pointConfigs[i].Speed());
                }
                items += posItems;
                fixItems = pointToItem.value((PointType)pointConfigs[i].Type());
                if(fixItems.size()){
                    items += fixItems;
                }
            }
        }
        else{
            propertyItems = propertyToItem.value((PointProperty)pointConfigs[i].Property());
            propertyItems.SetDVal(pointConfigs[i].Delay());
            items += propertyItems;
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

/**
 * @brief ICProgramPage::refreshCurrentRow
 * 1、M值 冲压允许 跟随点位
 * 2、夹吸 等待 冲压 跟随自己
 * @param step
 */
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

    int startIndex = 0,oldPoint = 0, pStart = 0;
    foreach(ICMoldItem item,allItems){
        if(item.GMVal() < ICMold::GX || item.GMVal() > ICMold::GC ){
            //M值跟随上一个位置
            if((item.GMVal() >=ICMold::GMWait &&  item.GMVal()< ICMold::GEuOut )
                    || item.GMVal() == ICMold::GCondition
                    || (item.GMVal() == ICMold::GEuOut && item.SubNum() == 0)){
                         stepToRow.insert(item.Num(),startIndex); //冲压允许保持跟随上一个位置
            }
            else{
                if(pStart == 0){
                    //等待，夹吸跟随当前位置
                    stepToRow.insert(item.Num(),startIndex++);
                }
                else{
                    //等待，夹吸跟随下一个位置
                    stepToRow.insert(item.Num(),++startIndex);
                }
            }
        }
        else{
            pStart = 1;
            stepToRow.insert(item.Num(),startIndex);
            if(oldPoint != item.SubNum()){
                startIndex++;
            }
            oldPoint = item.SubNum();

        }
    }

    quint32 row = stepToRow.value(step);
    if(row < ui->tableWidget->rowCount())
        ui->tableWidget->setCurrentIndex(ui->tableWidget->model()->index(row,0));

}

QTableWidget *ICProgramPage::TableWidget()
{
    return ui->tableWidget;
}

QList<ICMoldItem> ICProgramPage::MK_PosItem(int row,int pos)
{
    QList<ICMoldItem> items;

    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY1)) ==
            ICVirtualHost::ICAxisDefine_Servo){
#ifdef TEACH_3D
        items << MK_MoldItem(1,1,pos,ICMold::GARC,0,64,ICMold::GY,100,0,172);  //3D教导
#else
        items << MK_MoldItem(1,1,pos,ICMold::GY,0,64,0,100,0,172);  //组合教导
#endif
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX1)) ==
            ICVirtualHost::ICAxisDefine_Servo){
#ifdef TEACH_3D
        items  << MK_MoldItem(2,1,pos,ICMold::GARC,0,64,ICMold::GX,100,0,174);//3D教导
#else
        items  << MK_MoldItem(2,1,pos,ICMold::GX,0,64,0,100,0,174);//组合教导
#endif
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisZ)) ==
            ICVirtualHost::ICAxisDefine_Servo){
#ifdef TEACH_3D
        items  << MK_MoldItem(3,1,pos,ICMold::GARC,0,64,ICMold::GZ,100,0,176);  //3D教导
#else
        items  << MK_MoldItem(3,1,pos,ICMold::GZ,0,64,0,100,0,176);  //组合教导
#endif
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX2)) ==
            ICVirtualHost::ICAxisDefine_Servo){
#ifdef TEACH_3D
        items  << MK_MoldItem(4,1,pos,ICMold::GARC,0,64,ICMold::GP,100,0,178);  //3D教导
#else
        items  << MK_MoldItem(4,1,pos,ICMold::GP,0,64,0,100,0,178);  //组合教导
#endif
    }
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY2)) ==
            ICVirtualHost::ICAxisDefine_Servo){
#ifdef TEACH_3D
        items  << MK_MoldItem(5,1,pos,ICMold::GARC,0,64,ICMold::GQ,100,0,180);  //3D教导
#else
        items  << MK_MoldItem(5,1,pos,ICMold::GQ,0,64,0,100,0,180);  //组合教导
#endif
    }

    PointType t = (PointType)pointConfigs[row].Type();
    quint32 d = pointConfigs[row].Smooth();

    if(t != Point_Property){
        for(int i = 0; i < items.size();i++){
            items[i].SetSmooth(d);
            if(t == Get_Up ||
               t == Put_Up)
            {
                items[i].SetEarlySpeedDown(d);
            }
        }
    }
    return items;
}

ICProgramPage::~ICProgramPage()
{
    delete ui;
    for(int i=0;i<AXIS_COUNTS;i++)
        delete validators_[i];
    delete dValidator;
}


quint32 ICProgramPage::GetPointValue(quint16 pos)
{
     ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    quint32 s  = host->HostStatus(ICVirtualHost::DbgB0).toUInt() << 16;
    s = s + host->HostStatus(ICVirtualHost::DbgA1).toUInt();

    return ( (s >>( (pos -  ICVirtualHost::XPos)* 4 ) )& 0xF);
}

qint32 ICProgramPage::GetPosValue(ICVirtualHost::ICStatus status)
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    qint16  p =  host->HostStatus(status).toInt() ;
    if(p < 0){
       qint32 v = p * 10 -  GetPointValue(status) ;
        return v;
    }
    else{
        qint32 v = p * 10 + GetPointValue(status) ;
         return v;
    }
}



void ICProgramPage::showEvent(QShowEvent *e)
{
    timerId = startTimer(200);
    //隐藏列
    for(int i=0;i < AXIS_COUNTS;i++){
        if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX1 + i)) !=
           ICVirtualHost::ICAxisDefine_Servo)
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
        validators_.at(i)->setTop(top * 10);
        validators_.at(i)->setBottom(bottom * 10);
    }

    QWidget::showEvent(e);
}

void ICProgramPage::hideEvent(QHideEvent *e)
{
    killTimer(timerId);
    ui->startEdit->setChecked(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    SaveConfigPoint();
    ReConfigure();
    EnableTestButtons();

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
                        << tr("Rollovers")/*  << tr("Save") << tr("Tested")*/;

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

void ICProgramPage::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    static ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();

    ICUserActionInfo info;
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();

    for(int i =0;i < manualButtons.size();i++){
        int p;
        info = config->GetActionShortcutByID(i);

        if((info.type + ICMold::GOutY) == ICMold::GTwoXTwoY){
            p = info.pointNum * 2;
        }
        else if((info.type + ICMold::GOutY) == ICMold::GEuOut){
            p = info.pointNum + 64;
        }
        else{
            p = info.pointNum;
        }

        if(host->IsOutputOn(p))
        {
            manualButtons.at(i)->setStyleSheet(yOnStyle);
        }
        else
        {
            manualButtons.at(i)->setStyleSheet(oriStyle);
        }
    }


}

void ICProgramPage::itemClicked(QTableWidgetItem *item)
{
    if(!ui->startEdit->isChecked())
        return;

    if((item->row() >=0 && item->row() < ROW_COUNTS)  &&
        (item->column() >0 && item->column() < AXIS_COUNTS + 1)){

        if(pointConfigs[item->row()].Type() == Point_Property){

            ICMoldItem moldItem = propertyToItem.value((PointProperty)pointConfigs[item->row()].Property());

            if((moldItem.GMVal() == ICMold::GOneXOneY ||
               moldItem.GMVal() == ICMold::GCheckX) && item->column() == 1)

            {

                if(!_dialog->isVisible()){
                    _dialog->move(200,200);
                    _dialog->ResetDisplay();
                    _dialog->exec();
                }


                QString text = _dialog->GetCurrentText();
                int pointSize;
                if(!text.isEmpty() && (text != item->text())){
                    int value;
                    if(text.contains(QChar('.'))){
                        pointSize = text.split(".").at(1).size();
                        if(pointSize > DELAY_DECIMAL || pointSize==0){
                            QMessageBox::information(this,tr("information"),tr("Input Error!"));
                            _dialog->ResetDisplay();
                            return;
                        }
                        else{
                            value = text.remove(".").toInt() * qPow(10,DELAY_DECIMAL - pointSize);
                        }

                    }
                    else{
                        value = text.toInt() * qPow(10,DELAY_DECIMAL);
                    }


                    if(value > dValidator->top() || value < dValidator->bottom()){
                        QString format = QString("%.%1f").arg(DELAY_DECIMAL);
                        QString bottom =   QString().sprintf(format.toAscii(), dValidator->bottom() / static_cast<qreal>(qPow(10, DELAY_DECIMAL)));
                        QString top =   QString().sprintf(format.toAscii(), dValidator->top() / static_cast<qreal>(qPow(10, DELAY_DECIMAL)));

                        QMessageBox::information(this,tr("information"),tr("Input Value Not %1 To %2 Range!")
                                                 .arg(bottom)
                                                 .arg(top));
                        _dialog->ResetDisplay();
                        return;
                    }
                    item->setText(ICParameterConversion::TransThisIntToThisText(value, DELAY_DECIMAL) + tr("s"));
                    pointConfigs[item->row()].setDelay(value);
                    SaveConfigPoint();

                }

                return;

            }
            else{
                QMessageBox::information(this,tr("information"),
                                         tr("%1Can not Edit Point!")
                                         .arg(_typeDialog->toString((PointProperty)pointConfigs[item->row()].Property())));

                return;
            }
        }

        if(!_dialog->isVisible()){
            _dialog->move(200,200);
            _dialog->ResetDisplay();
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
                    if(POINT_SIZE != pointSize){
                        value = text.remove(".").toInt() * qPow(10,POINT_SIZE - pointSize);
                    }
                    else{
                        value = text.remove(".").toInt() ;
                    }
                }

            }
            else{
                value = text.toInt() * qPow(10,POINT_SIZE);
            }


            QIntValidator *v = validators_[column-1];
//            qDebug() << v->bottom() << v->top();
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
            //调用点保存和下发
            SaveConfigPoint();
        }


    }

    if(item->column() == DELAY_COLUMN ||
        item->column() == SPEED_COLUMN     ){
        if(pointConfigs[item->row()].Type() == Point_Property){
            return;
        }
        if(!_dialog->isVisible()){
            _dialog->move(200,200);
            _dialog->ResetDisplay();
            _dialog->exec();
        }
        QString text = _dialog->GetCurrentText();

        if(!text.isEmpty() && (text != item->text())){

            if(item->column() == DELAY_COLUMN){
                int value;
                if(text.contains(QChar('.'))){
                    int pointSize = text.split(".").at(1).size();
                    if(pointSize > DELAY_DECIMAL || pointSize==0){
                        QMessageBox::information(this,tr("information"),tr("Input Error!"));
                        _dialog->ResetDisplay();
                        return;
                    }
                    else{
                        if(DELAY_DECIMAL != pointSize){
                            value = text.remove(".").toInt() * qPow(10,DELAY_DECIMAL - pointSize);
                        }
                        else{
                            value = text.remove(".").toInt() ;
                        }
                    }

            }
            else{
                value = text.toInt() * qPow(10,DELAY_DECIMAL);
            }
            if(value > dValidator->top() || value < dValidator->bottom()){
                QString format = QString("%.%1f").arg(DELAY_DECIMAL);
                QString bottom =   QString().sprintf(format.toAscii(), dValidator->bottom() / static_cast<qreal>(qPow(10, DELAY_DECIMAL)));
                QString top =   QString().sprintf(format.toAscii(), dValidator->top() / static_cast<qreal>(qPow(10, DELAY_DECIMAL)));

                QMessageBox::information(this,tr("information"),tr("Input Value Not %1 To %2 Range!")
                                         .arg(bottom)
                                         .arg(top));
                _dialog->ResetDisplay();
                return;
            }
            item->setText(ICParameterConversion::TransThisIntToThisText(value, DELAY_DECIMAL) + "s");
            pointConfigs[item->row()].setDelay(value);
            SaveConfigPoint();
        }
        else  if(item->column() == SPEED_COLUMN){
                if(text.contains(QChar('.'))){
                        QMessageBox::information(this,tr("information"),tr("Input Error!"));
                        _dialog->ResetDisplay();
                        return;
                 }
                else{
                     int value = text.toInt();
                     if(value <1 || value > 100){
                         QMessageBox::information(this,tr("information"),tr("Input Value Not %1 To %2 Range!")
                                                  .arg(1)
                                                  .arg(100));
                         return;
                     }
                     item->setText(text + "%");
                     pointConfigs[item->row()].setSpeed(value);
                }
            }
       }

    }

#ifdef MODIFY_POINTNAME
    if(item->column() == 0){
                 _itemDialog->SetTextEditor(item);
                if(!_itemDialog->isVisible()){
                    _itemDialog->move(10,75);
                    _itemDialog->Reset();
                    _itemDialog->exec();
                }
    }
#endif

}

void ICProgramPage::saveButtonsCliked()
{
    int index = ui->tableWidget->currentIndex().row();
    if(index < 0 || index == ROW_COUNTS)
    {
        return;
    }

    if(pointConfigs[index].Type() == Point_Property){
        QMessageBox::information(this,tr("information"),
                                 tr("%1Can not Setting Point!")
                                 .arg(_typeDialog->toString((PointProperty)pointConfigs[index].Property())));
        return;
    }

    for(int i=0; i < AXIS_COUNTS;i++){
        int32_t pos = GetPosValue(ICVirtualHost::ICStatus(ICVirtualHost::XPos + i));
        ui->tableWidget->item(index,1+i)->setText(QString::number(pos / 100.0, 'f', 2));
//        ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(GT_PointIndexFromRow(index) * 6 + i),pos);

    }
//    ICMold::CurrentMold()->SaveMoldParamsFile();
    //调用点保存和下发
    SaveConfigPoint();

}

void ICProgramPage::testButonsPressed()
{
//    ui->testButton->setEnabled(false);
    int index = ui->tableWidget->currentIndex().row();

    if(index < 0  || index == ROW_COUNTS )
    {
        return;
    }

    if(pointConfigs[index].Type() == Point_Property){
        ICMoldItem moldItem = propertyToItem.value((PointProperty)pointConfigs[index].Property());
        if(moldItem.GMVal() == ICMold::GOneXOneY){
            ICManualRun cmd;
            cmd.SetSlave(1);
            cmd.SetGM(ICMold::GOneXOneY);
            cmd.SetPoint(moldItem.SubNum());
            cmd.SetIFVal(moldItem.IFVal());
            if(!ICCommandProcessor::Instance()->ExecuteCommand(cmd).toBool())
            {
//                QMessageBox::warning(this,
//                                         tr("warning"),
//                                         tr("Execute Manual Cmd failed!"));
            }
        }
        else{
            QMessageBox::information(this,tr("information"),
                                     tr("%1Can not Test Point!")
                                     .arg(_typeDialog->toString((PointProperty)pointConfigs[index].Property())));
        }

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
    cmd.SetPoint(GT_PointIndexFromRow(index));
    //    cmd.SetIFVal(1);
    ICCommandProcessor::Instance()->ExecuteCommand(cmd);
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
                    << tr("Rollovers")  << tr("Delay") << tr("Speed");

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

    int pointCount=0,start=0;

    pointConfigs = ICMold::CurrentMold()->MoldPointConfig();
    for(int i=0;i<pointConfigs.size();i++){
        ui->tableWidget->insertRow(ROW_COUNTS);

        if(pointConfigs[i].Type() != Point_Property){
            pointCount ++;
        }

    }


    //初始化点位
    for(int i=0;i<pointCount;i++){
        allPoints.append(MK_Point(_BUILD_INT32(_MoldParam(ICMold::point0 + i * 12 + 0 ),_MoldParam(ICMold::point0 + i * 12 + 1 )),
                                                      _BUILD_INT32(_MoldParam(ICMold::point0 + i * 12 + 2 ),_MoldParam(ICMold::point0 + i * 12 + 3 )),
                                                      _BUILD_INT32(_MoldParam(ICMold::point0 + i * 12 + 4 ),_MoldParam(ICMold::point0 + i * 12 + 5 )),
                                                      _BUILD_INT32(_MoldParam(ICMold::point0 + i * 12 + 6 ),_MoldParam(ICMold::point0 + i * 12 + 7 )),
                                                      _BUILD_INT32(_MoldParam(ICMold::point0 + i * 12 + 8),_MoldParam(ICMold::point0 + i * 12 + 9 ))
                                  ));

    }

    //重新设置行高
    for(int k=0;k< ui->tableWidget->verticalHeader()->count();k++){
        ui->tableWidget->verticalHeader()->resizeSection(k,ROW_HIGHT);
    }


    for(int j =0; j< pointConfigs.size();j++){
        for(int i=0;i <COLUMN_COUNTS ; i++){
            if(!ui->tableWidget->cellWidget(j,i) && !ui->tableWidget->item(j,i)){
                QTableWidgetItem * item = new QTableWidgetItem();
                item->setText("");
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->tableWidget->setItem(j,i,item);
            }
        }
        if(pointConfigs[j].Type() != Point_Property){
            if(pointConfigs[j].Reserve() == "0"){
                ui->tableWidget->item(j,0)->setText(_typeDialog->toString((PointType)pointConfigs[j].Type()));
            }
            else{
                ui->tableWidget->item(j,0)->setText(pointConfigs[j].Reserve() );
            }

        }
        else{
            ui->tableWidget->item(j,0)->setText(_typeDialog->toString((PointProperty)pointConfigs[j].Property()));

            ui->tableWidget->item(j,1)->setText(QString("%1s")
                                                .arg(ICParameterConversion::TransThisIntToThisText(pointConfigs[j].Delay(), DELAY_DECIMAL)));

        }
    }


    for(int i=0;i<pointConfigs.size();i++){
        if(pointConfigs[i].Type() != Point_Property){
            ui->tableWidget->item(i,1)->setText(QString::number(allPoints[start]->x / qPow(10,POINT_SIZE), 'f', POINT_SIZE));
            ui->tableWidget->item(i,2)->setText(QString::number(allPoints[start]->y / qPow(10,POINT_SIZE), 'f', POINT_SIZE));
            ui->tableWidget->item(i,3)->setText(QString::number(allPoints[start]->s / qPow(10,POINT_SIZE), 'f', POINT_SIZE));
            ui->tableWidget->item(i,4)->setText(QString::number(allPoints[start]->r / qPow(10,POINT_SIZE), 'f', POINT_SIZE));
            ui->tableWidget->item(i,5)->setText(QString::number(allPoints[start]->t / qPow(10,POINT_SIZE), 'f', POINT_SIZE));
            ui->tableWidget->item(i,6)->setText(ICParameterConversion::TransThisIntToThisText(pointConfigs[i].Delay(), DELAY_DECIMAL)+ "s");
            ui->tableWidget->item(i,7)->setText(QString::number(pointConfigs[i].Speed(), 'f', 0) + "%");

            start++;
        }
        else{
//            ui->tableWidget->item(i,1)->setText("-");
            ui->tableWidget->item(i,2)->setText("-");
            ui->tableWidget->item(i,3)->setText("-");
            ui->tableWidget->item(i,4)->setText("-");
            ui->tableWidget->item(i,5)->setText("-");
            ui->tableWidget->item(i,6)->setText("-");
            ui->tableWidget->item(i,7)->setText("-");
        }
    }

    //初始化平滑
    for(int i=0; i <pointConfigs.size();i++){
        PointType t = (PointType)pointConfigs[i].Type();
        if(t != Point_Property){
            quint32 d = pointConfigs[i].Smooth();
            SetRowSMooth(i,d);
        }
    }
}

void ICProgramPage::DisableTestButtons()
{
    ui->testButton->setEnabled(false);
}

void ICProgramPage::EnableTestButtons()
{
    ui->testButton->setEnabled(true);

}

void ICProgramPage::DeleteWidgets()
{
    int rowCount = ROW_COUNTS;
    for(int index=0;index < rowCount;index++){
        for(int i=0;i < ui->tableWidget->columnCount();i++){
            if(ui->tableWidget->item(index,i))
            delete ui->tableWidget->item(index,i);
        }

        ui->tableWidget->removeRow(0);
        pointTypes.removeAt(0);
    }
//    for(int k=0;k< ui->tableWidget->verticalHeader()->count();k++){
//        ui->tableWidget->verticalHeader()->resizeSection(k,ROW_HIGHT);
//    }

}

void ICProgramPage::InitPointToItem()
{
    QList<ICMoldItem> items;
    pointToItem.clear();
    pointToItem.insert(Get_Wait,(items << waitM10)); items.clear();  //第一个取料待机
    pointToItem.insert(Get_Up,items); items.clear();
    pointToItem.insert(Get,items); items.clear();
    pointToItem.insert(Get_Wait3,items); items.clear();
    pointToItem.insert(Get_Wait2,(items << outM11 <<  outM27On)); items.clear();    //第二个取料待机
    pointToItem.insert(Put_Wait,(items << waitM12)); items.clear();    //第一个放料待机
    pointToItem.insert(Put_Up,items); items.clear();
    pointToItem.insert(Put,items); items.clear();
    pointToItem.insert(Reserve,items); items.clear();
    pointToItem.insert(Wait_Safe,items << checkM15); items.clear();

//    qDebug() << ICMold::CurrentMold()->MoldName();
    //第二个放料待机
    if(ICMold::CurrentMold()->MoldNativeParam(ICMold::moldType) == ICMold::punchType){
        pointToItem.insert(Put_Wait2,(items << outPermit << waitM10)); items.clear();
    }
    else{
        pointToItem.insert(Put_Wait2,(items << outPermit)); items.clear();
    }




    propertyToItem.clear();
    propertyToItem.insert(OUYY37_ON,outY37On);
    propertyToItem.insert(OUYY37_OFF,outY37Off);
    propertyToItem.insert(OUYY40_ON,outY40On);
    propertyToItem.insert(OUYY40_OFF,outY40Off);
    propertyToItem.insert(OUYY22_ON,outY22On);
    propertyToItem.insert(OUYY22_OFF,outY22Off);
    propertyToItem.insert(OUYY23_ON,outY23On);
    propertyToItem.insert(OUYY23_OFF,outY23Off);
    propertyToItem.insert(WAIT_X41,waitX41);
    propertyToItem.insert(WAIT_X42,waitX42);
    propertyToItem.insert(WAIT_X43,waitX43);
    propertyToItem.insert(WAIT_X44,waitX44);
    propertyToItem.insert(PUNCHPERMIT,outPermit2);



}

void ICProgramPage::SaveConfigPoint()
{
    //保存点配置文件
    if(PointConfigChanged(pointConfigs)){
        ICMold::CurrentMold()->SetMoldPointConfig(pointConfigs);
        ICMold::CurrentMold()->SaveMoldPointFile();
    }

    bool saved = false;
    int start = 0;
    //保存点坐标
    for(int i=0;i<ROW_COUNTS;i++){
        for(int j=0;j<AXIS_COUNTS;j++){
            if(pointConfigs[i].Type() != Point_Property){
                int32_t pos = ui->tableWidget->item(i,j+1)->text().remove(".").toInt();
                int32_t oldPos  = _BUILD_INT32( _MoldParam(start * 12 + j * 2) ,  _MoldParam(start * 12 + j * 2 + 1)) ;
                if(pos != oldPos){
                    saved = true;
                    _SetMoldParam(start * 12 + j * 2,_LOW16_INT32(pos));
                    _SetMoldParam(start * 12 + j * 2  + 1,_HIGH16_INT32(pos));
                }
            }
        }
        if(pointConfigs[i].Type() != Point_Property){
            start++;
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



PointPtr ICProgramPage::MK_Point(quint32 x, quint32 y, quint32 s, quint32 r, quint32 t)
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
    checkM15  = MK_MoldItem(3,1,5,14,0,129,0,2,0,154);
    outM11    = MK_MoldItem(8,4,1,25,0,1,0,0,0,204);
    waitM12   = MK_MoldItem(9,5,2,24,0,1,0,0,3000,207);
    waitM14   = MK_MoldItem(13,9,4,24,0,1,0,0,3000,246);
    outPermit = MK_MoldItem(14,10,0,27,0,1,0,0,0,103);
    outPermit2 = MK_MoldItem(14,10,1,27,0,1,0,0,0,103);


    outY37On  = MK_MoldItem(7,3,23,12,0,1,0,0,0,45);
    outY37Off = MK_MoldItem(10,6,23,12,0,0,0,0,0,50);
    outY40On  = MK_MoldItem(7,3,24,12,0,1,0,0,0,45);
    outY40Off = MK_MoldItem(7,3,24,12,0,0,0,0,0,45);
    outY22On  = MK_MoldItem(7,3,10,12,0,1,0,0,0,45);
    outY22Off = MK_MoldItem(7,3,10,12,0,0,0,0,0,45);
    outY23On  = MK_MoldItem(7,3,11,12,0,1,0,0,0,45);
    outY23Off = MK_MoldItem(7,3,11,12,0,0,0,0,0,45);

    waitX41   = MK_MoldItem(9,9,25,10,0,1,0,0,3000,54);
    waitX42   = MK_MoldItem(10,10,26,10,0,1,0,0,3000,57);
    waitX43   = MK_MoldItem(9,9,27,10,0,1,0,0,3000,54);
    waitX44   = MK_MoldItem(10,10,28,10,0,1,0,0,3000,57);


    //遗弃
    outM27On  = MK_MoldItem(13,9,15,25,0,1,0,0,0,246);


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
    if(_typeDialog->exec() == QDialog::Accepted){
        if(GT_PointCount() == MAX_ROWCOUNT && _typeDialog->currentPropertyIsPoint()){
            QMessageBox::information(this,tr("Information"),tr("Max Point Count Beyond %1!").arg(MAX_ROWCOUNT));
            return;
        }

        if(_typeDialog->currentPropertyType() == NULL_Property){
            return;
        }
        else if(_typeDialog->currentPropertyType() == SMOOTH){
            quint32 t = pointConfigs[index].Type();
            quint32 s = pointConfigs[index].Smooth();
            if(t != Point_Property){
                if(s == 1)  //动作重复
                    return;
                pointConfigs[index].setSmooth(1); //设置平滑
                SetRowSMooth(index,true);
            }
            else{
                QMessageBox::warning(this,tr("warning"),
                                     tr("Not wait point Can Set Smooth!"));
                return;
            }
            return;
        }
        ui->tableWidget->insertRow(index);
        ui->tableWidget->setItem(index,0,new QTableWidgetItem(_typeDialog->toString(_typeDialog->currentPropertyType())));


        ui->tableWidget->item(index,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        pointConfigs.insert(index,_typeDialog->config());


        for(int i = 0 ;i < AXIS_COUNTS; i++){
            QTableWidgetItem *item = new QTableWidgetItem("") ;
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->setItem(index,i+1,item);
            if(_typeDialog->currentPropertyType() != RESEARVE &&
               _typeDialog->currentPropertyType() != WAITSAFE){
                if(i==0){
                    ui->tableWidget->item(index,i+1)->setText(QString("%1s")
                                                        .arg(ICParameterConversion::TransThisIntToThisText(pointConfigs[index].Delay(), DELAY_DECIMAL)));


                }
                else{
                    item->setText("-");
                }
            }
            else{
                item->setText(ICParameterConversion::TransThisIntToThisText(0, POINT_SIZE));
            }
        }
        for(int i=AXIS_COUNTS+1;i < COLUMN_COUNTS;i++){
            QTableWidgetItem *item = new QTableWidgetItem("") ;
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->setItem(index,i,item);
            if(_typeDialog->currentPropertyType() != RESEARVE &&
               _typeDialog->currentPropertyType() != WAITSAFE){
                    item->setText("-");
            }
            else{
                if(i == AXIS_COUNTS+1 ){
                    item->setText(ICParameterConversion::TransThisIntToThisText(pointConfigs[index].Delay(), DELAY_DECIMAL) +"s");
                }
                else{
                    item->setText("100%");
                    pointConfigs[index].setSpeed(100);
                }
            }
        }

        for(int k=0;k< ui->tableWidget->verticalHeader()->count();k++){
            ui->tableWidget->verticalHeader()->resizeSection(k,ROW_HIGHT);
        }
        DisableTestButtons();

    }

}

void ICProgramPage::on_modiifyButton_clicked()
{
//    int index = ui->tableWidget->currentIndex().row();
//    if(index == -1) index ++;
//    if(index == ROW_COUNTS){
//        return;
//    }
//    _propertyDialog->SetPointProperty(pointProperty.at(index));
//    if(_propertyDialog->exec() == QDialog::Accepted){
//        pointProperty[index] = _propertyDialog->GetPointProperty();
//    }
}

void ICProgramPage::on_deleteButton_clicked()
{

    int index = ui->tableWidget->currentIndex().row();
    if(index == -1) index ++;

    if(index == ui->tableWidget->rowCount() -1){
        return;
    }

    QString moldName = ICMold::CurrentMold()->MoldName().split("/")[2];
    if(standPrograms_.contains(moldName)){
        QMessageBox::information(this,tr("Information"),tr("Standard Mold Cannot Modify!"));
        return;
    }


    PointType t = (PointType)pointConfigs[index].Type();
    quint32 d = pointConfigs[index].Smooth();

    if(t != Point_Property  && d){
        if(QMessageBox::information(this,tr("Information"),tr("IS Delete %1 Smooth Action !")
                        .arg( _typeDialog->toString(t)),
                        QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok){
            return;
        }
        pointConfigs[index].setSmooth(0);
        SetRowSMooth(index,false);
        return;
    }

//#ifdef Q_WS_QWS
    if(pointConfigs[index].Type() == Get_Wait ||
        pointConfigs[index].Type() == Get ||
        pointConfigs[index].Type() == Get_Up ||
        pointConfigs[index].Type() == Get_Wait2 ||
        pointConfigs[index].Type() == Get_Wait3 ||
        pointConfigs[index].Type() == Put_Wait ||
        pointConfigs[index].Type() == Put ||
        pointConfigs[index].Type() == Put_Up ||
        pointConfigs[index].Type() == Put_Wait2 ){
        QMessageBox::information(this,tr("Information"),tr("Canot Delete %1 action!").arg( _typeDialog->toString((PointType)pointConfigs[index].Property() )));
        return;
    }
//#endif

    if(QMessageBox::information(this,tr("Information"),tr("If Delete current Row?"),
                                QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok)
    {
        return;
    }

    if(index != ROW_COUNTS){
        for(int i=0;i < ui->tableWidget->columnCount();i++){
            delete ui->tableWidget->item(index,i);
        }

        pointConfigs.removeAt(index);
        ui->tableWidget->removeRow(index);

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
    QMessageBox::information(this,tr("Information"),tr("Save success!"));
}

void ICProgramPage::MoldChanged(QString s)
{
    InitPoint();
    InitPointToItem();
    allItems = GT_AllMoldItems();

}

void ICProgramPage::OnShortcutTriggered()
{
    int id = manualButtons.indexOf(qobject_cast<QPushButton*>( sender()));
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    ICUserActionInfo info = config->GetActionShortcutByID(id);

    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GOutY + info.type);
    cmd.SetPoint(info.pointNum);
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int p;
    if(cmd.GM() == ICMold::GTwoXTwoY){
        p = info.pointNum * 2;
    }
    else if(cmd.GM() == ICMold::GEuOut){
        p = info.pointNum + 64;
    }
    else{
        p = info.pointNum;
    }
    if(host->IsOutputOn(p))
    {
        cmd.SetIFVal(0);
    }
    else
    {
        cmd.SetIFVal(1);
    }
    //    cmd.SetIFVal(info.dir);
    if(!ICCommandProcessor::Instance()->ExecuteCommand(cmd).toBool())
    {
//        QMessageBox::warning(this,
//                                 tr("warning"),
//                                 tr("err"));
    }
}

void ICProgramPage::OnShortcutReleased()
{
    int id = manualButtons.indexOf(qobject_cast<QPushButton*>( sender()));
    ICUserDefineConfigSPTR config = ICUserDefineConfig::Instance();
    ICUserActionInfo info = config->GetActionShortcutByID(id);
    if(info.type != 0) return;
    if(info.pointNum != 10 && info.pointNum != 11) return;
    ICManualRun cmd;
    cmd.SetSlave(1);
    cmd.SetGM(ICMold::GOutY + info.type);
    cmd.SetPoint(info.pointNum);
    cmd.SetIFVal(0);
    if(!ICCommandProcessor::Instance()->ExecuteCommand(cmd).toBool())
    {
//        QMessageBox::warning(this,
//                                 tr("warning"),
//                                 tr("err"));
    }
}


void ICProgramPage::GT_CalculateItem(QList<ICMoldItem>& items)
{
    uint oldNum = 0;
    //计算num
    for(int i=0;i<items.size();i++){
#ifdef TEACH_3D
        if(items.at(i).GMVal() == ICMold::GARC)
#else
        if(items.at(i).GMVal() >= ICMold::GX && items.at(i).GMVal() <= ICMold::GC)
#endif
        {
            items[i].SetNum(oldNum);
            if(items.at(i).GMVal()  == GT_LastAxisNum()){
                oldNum ++;
            }

        }
        else if(items.at(i).GMVal() == ICMold::GEuOut){
            if(LastIsSmooth()){
                items[i].SetNum(oldNum-1);
            }
            else {
                items[i].SetNum(oldNum);
                oldNum ++;
            }
        }
        else{
            items[i].SetNum(oldNum);
            oldNum ++;
        }

    }
}

void ICProgramPage::SetRowSMooth(int index, bool smooth)
{
    PointType t = (PointType)(pointConfigs[index].Type());
    QString s  = pointConfigs[index].Reserve();
    if(s == "0"){
        s  = ICPointType::Instance()->toString(t);
    }
    else{
        s = pointConfigs[index].Reserve();
    }
    if(smooth)
        s += tr("(SMOOTH)");
    ui->tableWidget->item(index,0)->setText(s);
}

quint32 ICProgramPage::GT_LastAxisNum()
{
    uint lastAxis = 0;
    if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY2)) ==
            ICVirtualHost::ICAxisDefine_Servo){
        lastAxis = 5;

    }
    else     if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX2)) ==
                ICVirtualHost::ICAxisDefine_Servo){
        lastAxis = 4;

    }
    else     if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisZ)) ==
                ICVirtualHost::ICAxisDefine_Servo){
        lastAxis = 3;

    }
    else     if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisX1)) ==
                ICVirtualHost::ICAxisDefine_Servo){
        lastAxis = 1;
    }
    else     if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis(ICVirtualHost::ICAxis_AxisY1)) ==
                ICVirtualHost::ICAxisDefine_Servo){
        lastAxis = 2;
    }

    return lastAxis;
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

bool ICProgramPage::PointConfigChanged(QList<ICPointConfig> &items)
{
    QList<ICPointConfig> oldItems = ICMold::CurrentMold()->MoldPointConfig();
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

bool ICProgramPage::LastIsSmooth()
{
    int size = pointConfigs.size();
    return (pointConfigs[size - 1].Smooth() == 1);
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

int ICProgramPage::GT_PointCount()
{
    int count = 0;
    for(int i=0;i<pointConfigs.size();i++){
        if(pointConfigs[i].Type() != Point_Property){
            count++;
        }
    }
    return count;
}

int ICProgramPage::GT_PointIndexFromRow(int row)
{
    int count = 0;
    for(int i=0;i<row;i++){
        if(pointConfigs[i].Type() != Point_Property){
            count++;
        }
    }
    return count;
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
