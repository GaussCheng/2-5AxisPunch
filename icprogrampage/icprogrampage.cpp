#include "icprogrampage.h"
#include "ui_icprogrampage.h"
#include <QDebug>
#include "icparameterssave.h"
#include "icmold.h"
#include <QMessageBox>
#include "moldinformation.h"


ICProgramPage::ICProgramPage(QWidget *parent,int _pageIndex) :
    QWidget(parent),
    ui(new Ui::ICProgramPage),
    _index(_pageIndex)
{
    ui->setupUi(this);
    _dialog =  VirtualNumericKeypadDialog::Instance();
    _typeDialog = new ICPointType(this);
    _host = ICVirtualHost::GlobalVirtualHost();

    InitTableWidget();
    InitPoint();
    InitFixMoldItems();
    InitPointToItem();
    connect(MoldInformation::Instance(),SIGNAL(MoldChanged(QString)),
            SLOT(MoldChanged(QString)));
}


QList<ICMoldItem> ICProgramPage::GT_MoldItems()
{
    QList<ICMoldItem> items;
    QList<ICMoldItem> fixItems;

    for(int i=0;i < ui->tableWidget->rowCount() -1;i++){
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

QList<ICMoldItem> ICProgramPage::MK_PosItem(int pos)
{
    QList<ICMoldItem> items;
    items << MK_MoldItem(1,1,pos,22,0,64,2,80,0,172)  //3D教导
        << MK_MoldItem(2,1,pos,22,0,64,1,80,0,174)
        << MK_MoldItem(3,1,pos,22,0,64,3,80,0,176)
        << MK_MoldItem(4,1,pos,22,0,64,4,80,0,178)
        << MK_MoldItem(5,1,pos,22,0,64,5,80,0,180);
    return items;
}

ICProgramPage::~ICProgramPage()
{
    delete ui;
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


    QWidget::showEvent(e);
}

void ICProgramPage::hideEvent(QHideEvent *e)
{
    ICMold::CurrentMold()->SaveMoldParamsFile();
    QWidget::hideEvent(e);

}

void ICProgramPage::itemClicked(QTableWidgetItem *item)
{
//    if((item->row() >=0 && item->row() < ui->tableWidget->rowCount() -1)  &&
//        (item->column() >0 && item->column() < AXIS_COUNTS + 1)){

//        if(!_dialog->isVisible()){
//            _dialog->move(200,200);
//            _dialog->exec();
//        }


//        QString text = _dialog->GetCurrentText();

//        if(!text.isEmpty() && (text != item->text())){
//            if(!text.contains(QChar('.'))){
//                text += ".0";
//            }
//            item->setText(text);
//        }
//    }

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

void ICProgramPage::testButonsClicked()
{

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

    ui->tableWidget->insertRow(0);
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(itemClicked(QTableWidgetItem*)));
}

void ICProgramPage::InitPoint()
{
    DeleteWidgets();

    uint pointCount = _MoldParam(ICMold::getPointCount + _index * 2);
    uint pointConfig = _MoldParam(ICMold::getPointConfig + _index * 2);

    for(int i =0 ;i < pointCount;i++){
        pointTypes.append((PointType)(pointConfig >> (i *4) & 0xF));
    }


    for(int i = 0 ;i < pointCount;i++){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount()-1);

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
        connect(testButtons[i],SIGNAL(clicked()),
                SLOT(testButonsClicked()));


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

void ICProgramPage::DeleteWidgets()
{
    int rowCount = ui->tableWidget->rowCount()-1;
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
}

void ICProgramPage::InitPointToItem()
{
    QList<ICMoldItem> items;
    pointToItem.insert(Get_Wait,(items << waitM10)); items.clear();
    pointToItem.insert(Get_Up,items); items.clear();
    pointToItem.insert(Get,(items << outY37On)); items.clear();
    pointToItem.insert(Get_Finish,(items << outM11 << waitM12)); items.clear();
    pointToItem.insert(Put_Wait,items); items.clear();
    pointToItem.insert(Put_Up,items); items.clear();
    pointToItem.insert(Put,(items << outY37Off)); items.clear();
    pointToItem.insert(Put_Finish,(items << waitM14 << outPermit)); items.clear();
    pointToItem.insert(Reserve,items); items.clear();


}

void ICProgramPage::SaveConfigPoint()
{
    //保存点配置
    int config = 0;
    for(int i=0;i<pointTypes.size();i++)
        config += (pointTypes.at(i)  << (i *4));

    if(_MoldParam(ICMold::getPointCount + _index * 2) != ui->tableWidget->rowCount() -1)
        _SetMoldParam(ICMold::getPointCount + _index * 2, ui->tableWidget->rowCount() -1);
    if(_MoldParam(ICMold::getPointConfig + _index * 2) != config)
        _SetMoldParam(ICMold::getPointConfig + _index * 2,config);

    //保存点坐标
    for(int i=0;i<ui->tableWidget->rowCount() - 1;i++){
        for(int j=0;j<AXIS_COUNTS;j++){
            int16_t pos = ui->tableWidget->item(i,j+1)->text().remove(".").toInt();
            int16_t oldPos = _MoldParam(_index * 6 * MAX_POINTS + i * 6 + j);
            if(pos != oldPos)
                _SetMoldParam(_index * 6 * MAX_POINTS + i * 6 + j,pos);

        }
    }
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
    waitM10   = MK_MoldItem(6,2,0,24,0,1,0,0,0,33);
    outY37On  = MK_MoldItem(7,3,23,11,0,1,0,0,0,45);
    outM11    = MK_MoldItem(8,4,1,25,0,1,0,0,30000,204);
    waitM12   = MK_MoldItem(9,5,2,25,0,1,0,0,30000,207);
    outY37Off = MK_MoldItem(10,6,23,11,0,0,0,0,0,50);
    outY31On  = MK_MoldItem(11,7,17,11,0,1,0,0,0,47);
    outY31Off = MK_MoldItem(12,8,17,11,0,0,0,0,50,98);
    waitM14   = MK_MoldItem(13,9,4,24,0,1,0,0,3000,246);
    outPermit = MK_MoldItem(14,10,3,25,0,1,0,0,50,103);

}

void ICProgramPage::on_newButton_clicked()
{
    int index = ui->tableWidget->currentIndex().row();
    if(index == -1) index ++;
    if(_typeDialog->exec() == QDialog::Accepted){

        if(ui->tableWidget->rowCount() == MAX_ROWCOUNT){
            return;
        }
        ui->tableWidget->insertRow(index);
        ui->tableWidget->setItem(index,0,new QTableWidgetItem(_typeDialog->toString()));
        ui->tableWidget->item(index,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        for(int i = 0 ;i < AXIS_COUNTS; i++){
            QTableWidgetItem *item = new QTableWidgetItem("") ;
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
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
        connect(testButtons[index],SIGNAL(clicked()),
                SLOT(testButonsClicked()));
        ui->tableWidget->setCellWidget(index,AXIS_COUNTS + 2,testButtons[index]);
        pointTypes.insert(index,_typeDialog->currentType());


        for(int k=0;k< ui->tableWidget->verticalHeader()->count();k++){
            ui->tableWidget->verticalHeader()->resizeSection(k,40);
        }
    }

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
    if(index != ui->tableWidget->rowCount() -1){
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
        qDebug() << "Not  Delete Space Line";
    }

}

void ICProgramPage::on_saveButton_clicked()
{
    SaveConfigPoint();
}

void ICProgramPage::MoldChanged(QString s)
{
    InitPoint();
}
