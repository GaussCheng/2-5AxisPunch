#include "icprogrampage.h"
#include "ui_icprogrampage.h"
#include <QDebug>

ICProgramPage::ICProgramPage(QWidget *parent,int _pageIndex) :
    QWidget(parent),
    ui(new Ui::ICProgramPage),
    _index(_pageIndex)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(MAX_POINTS + 1);
    ui->tableWidget->setColumnCount(COLUMN_COUNTS);


    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->hide();

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList headerContents;
    headerContents  << tr("UpDown") << tr("LeftRight") << tr("Rotated") << tr("Windup")
                       << tr("Rollovers") << tr("Used") << tr("Save") << tr("Tested");


    for(int i = 1 ;i < COLUMN_COUNTS;i++){
        QTableWidgetItem * item = new QTableWidgetItem();
        item->setText(headerContents.at(i-1));
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(0,i,item);
//        if(!ui->tableWidget->cellWidget(0,i)){
//            QLabel *label = new QLabel(headerContents.at(i-1));
//            label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//            ui->tableWidget->setCellWidget(0,i,new QLabel(headerContents.at(i-1)));
//        }
    }
    for(int i = 0 ;i < MAX_POINTS;i++){
        saveButtons[i].setText(tr("save"));
        saveButtons[i].setMinimumHeight(40);
        testButtons[i].setText(tr("test"));
        testButtons[i].setMinimumHeight(40);

        ui->tableWidget->setCellWidget(i + 1,7,&(saveButtons[i]));
        ui->tableWidget->setCellWidget(i + 1,8,&(testButtons[i]));
    }

    for(int i=0; i < MAX_POINTS - 3;i++){
        reserveButtons[i].setName("Skip");
        reserveButtons[i].setMinimumHeight(40);
        reserveButtons[i].setCheckedName("Used");

    }

    ui->tableWidget->setCellWidget(2,6,&(reserveButtons[0]));
    ui->tableWidget->setCellWidget(4,6,&(reserveButtons[1]));

    QStringList ql;
    ql << tr("first") << tr("reserve") << tr("second") << tr("reserve") << tr("third");
    setItemNames(ql);
//    itemClicked ( QTableWidgetItem * item ) [signal]
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(itemClicked(QTableWidgetItem*)));

    connect(ui->tableWidget,SIGNAL(itemPressed(QTableWidgetItem*)),
            this,SLOT(itemClicked(QTableWidgetItem*)));
}

void ICProgramPage::setItemNames(QStringList &contents)
{
    Q_ASSERT(contents.size() == 5);
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

ICProgramPage::~ICProgramPage()
{
    delete ui;
}

void ICProgramPage::itemClicked(QTableWidgetItem *item)
{
    qDebug() << item->row() << item->column();
}
