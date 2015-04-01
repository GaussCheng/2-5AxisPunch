#include "icprogrampage.h"
#include "ui_icprogrampage.h"
#include <QDebug>
#include "icparameterssave.h"
#include "icmold.h"



ICProgramPage::ICProgramPage(QWidget *parent,int _pageIndex) :
    QWidget(parent),
    ui(new Ui::ICProgramPage),
    _index(_pageIndex)
{
    ui->setupUi(this);
    _dialog =  VirtualNumericKeypadDialog::Instance();
    _host = ICVirtualHost::GlobalVirtualHost();

    InitTableWidget();
    InitPoints();
}

void ICProgramPage::setItemNames(QStringList &contents)
{
    Q_ASSERT(contents.size() == MAX_POINTS);
    for(int i = 0 ;i < MAX_POINTS;i++){
        if(ui->tableWidget->item(i+1,0)){
            ui->tableWidget->item(i+1,0)->setText(contents.at(i));

        }
        else{
            QTableWidgetItem * item = new QTableWidgetItem(contents.at(i));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->setItem(i+1,0,item);
        }
    }
}

QList<PointPtr> ICProgramPage::GT_Points()
{
    for(int i=0;i<MAX_POINTS;i++){
        allPoints[i]->x = ui->tableWidget->item(i+1,1)->text().remove(".").toInt();
        allPoints[i]->y = ui->tableWidget->item(i+1,2)->text().remove(".").toInt();
        allPoints[i]->s = ui->tableWidget->item(i+1,3)->text().remove(".").toInt();
        allPoints[i]->r = ui->tableWidget->item(i+1,4)->text().remove(".").toInt();
        allPoints[i]->t = ui->tableWidget->item(i+1,5)->text().remove(".").toInt();
    }
    return allPoints;
}

ICProgramPage::~ICProgramPage()
{
    delete ui;
}

void ICProgramPage::showEvent(QShowEvent *e)
{
    QList<PointPtr> p = GT_Points();


    int used = ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(ICMold::programInnerUsed));

    for(int i=0;i<MAX_POINTS - 3;i++){
        reserveButtons[i]->setChecked(used & ( 1 << (_index * 2 +  i)));
        saveButtons[reserveIndexs[i] -1]->setEnabled(used & ( 1 << (_index * 2 +  i)));
        testButtons[reserveIndexs[i] -1]->setEnabled(used & ( 1 << (_index * 2 +  i)));
    }

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

void ICProgramPage::itemClicked(QTableWidgetItem *item)
{
    if((item->row() >0 && item->row() < MAX_POINTS + 1)  &&
        (item->column() >0 && item->column() < 6)){

        if(!_dialog->isVisible()){
            _dialog->move(200,200);
            _dialog->exec();
        }

        QString text = _dialog->GetCurrentText();
        if(!text.isEmpty())
            item->setText(text);

    }



}

void ICProgramPage::saveButtonsCliked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int index = saveButtons.indexOf(button);

    for(int i=0; i < AXIS_COUNTS;i++){
        int16_t pos = _host->HostStatus(ICVirtualHost::ICStatus(ICVirtualHost::XPos + i)).toInt();
        ui->tableWidget->item(index+1,1+i)->setText(QString::number(pos / 10.0, 'f', 1));

    }
}

void ICProgramPage::testButonsClicked()
{

}

void ICProgramPage::usedButtonsClicked(bool checked)
{


    ICCheckedButton *button = qobject_cast<ICCheckedButton*>(sender());
    int index = reserveButtons.indexOf(button);
    int used = ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(ICMold::programInnerUsed));
    if(checked){
        used |= 1 <<  (_index * 2 +  index);
    }
    else{
        used &= ~(1 <<  (_index * 2 +  index));
    }
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(ICMold::programInnerUsed), used);
    ICMold::CurrentMold()->SaveMoldParamsFile();

    if(checked){
        saveButtons[reserveIndexs[index] -1]->setEnabled(true);
        testButtons[reserveIndexs[index] -1]->setEnabled(true);
    }
    else{
        saveButtons[reserveIndexs[index] -1]->setEnabled(false);
        testButtons[reserveIndexs[index] -1]->setEnabled(false);
        for(int i=0;i<AXIS_COUNTS;i++){
            ui->tableWidget->item(reserveIndexs[index],i+1)->setText("");
        }
    }

}



void ICProgramPage::on_pushButton_clicked()
{
    emit returnProgram();
}

void ICProgramPage::InitTableWidget()
{
    ui->tableWidget->setRowCount(MAX_POINTS + 1);
    ui->tableWidget->setColumnCount(COLUMN_COUNTS);


    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->hide();

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList headerContents;
    headerContents  << tr("FrontBack") << tr("UpDown") << tr("Windup") <<  tr("Rotated")
                    << tr("Rollovers") << tr("Used Stoped") << tr("Save") << tr("Tested");

    reserveIndexs << 2 << 4;

    for(int i = 1 ;i < COLUMN_COUNTS;i++){
        QTableWidgetItem * item = new QTableWidgetItem();
        item->setText(headerContents.at(i-1));
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(0,i,item);
    }
    for(int i = 0 ;i < MAX_POINTS;i++){
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

        ui->tableWidget->setCellWidget(i + 1,AXIS_COUNTS + 2,saveButtons[i]);
        ui->tableWidget->setCellWidget(i + 1,AXIS_COUNTS + 3,testButtons[i]);
    }

    for(int i=0; i < MAX_POINTS - 3;i++){
        ICCheckedButton *button = new ICCheckedButton(this);
        reserveButtons.append(button);
        reserveButtons[i]->setName(tr("Skip"));
        reserveButtons[i]->setFocusPolicy(Qt::NoFocus);
        reserveButtons[i]->setMinimumHeight(40);
        reserveButtons[i]->setCheckedName(tr("Used"));
        connect(reserveButtons[i],SIGNAL(clicked(bool)),
                SLOT(usedButtonsClicked(bool)));
    }

    for(int i=0 ; i < reserveIndexs.count();i++){
        int index = reserveIndexs[i];
        ui->tableWidget->setCellWidget(index,AXIS_COUNTS + 1,reserveButtons[i]);
        saveButtons[index - 1]->setEnabled(false);
        testButtons[index - 1]->setEnabled(false);

    }

    for(int j =0; j< MAX_POINTS + 1;j++){
        for(int i=0;i <COLUMN_COUNTS ; i++){
            if(!ui->tableWidget->cellWidget(j,i) && !ui->tableWidget->item(j,i)){
                QTableWidgetItem * item = new QTableWidgetItem();
                item->setText("");
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->tableWidget->setItem(j,i,item);            }
        }
    }

    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(itemClicked(QTableWidgetItem*)));
}

void ICProgramPage::InitPoints()
{
    for(int i=0;i<MAX_POINTS;i++){
        ui->tableWidget->item(i+1,1)->setText(QString::number(allPoints[i]->x / 10.0, 'f', 1));
        ui->tableWidget->item(i+1,2)->setText(QString::number(allPoints[i]->y / 10.0, 'f', 1));
        ui->tableWidget->item(i+1,3)->setText(QString::number(allPoints[i]->s / 10.0, 'f', 1));
        ui->tableWidget->item(i+1,4)->setText(QString::number(allPoints[i]->r / 10.0, 'f', 1));
        ui->tableWidget->item(i+1,5)->setText(QString::number(allPoints[i]->t / 10.0, 'f', 1));
    }

}

PointPtr ICProgramPage::MK_Point(qint16 x, qint16 y, qint16 s, qint16 r, qint16 t)
{
    PointPtr p(new ICPoint);
    p->x = x;
    p->y = y;
    p->s = s;
    p->r = r;
    p->t = t;

    return p;
}
