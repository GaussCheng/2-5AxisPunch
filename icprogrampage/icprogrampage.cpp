#include "icprogrampage.h"
#include "ui_icprogrampage.h"
#include <QDebug>
#include "icparameterssave.h"


ICProgramPage::ICProgramPage(QWidget *parent,int _pageIndex) :
    QWidget(parent),
    ui(new Ui::ICProgramPage),
    _index(_pageIndex)
{
    ui->setupUi(this);
    _dialog =  VirtualNumericKeypadDialog::Instance();
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
                       << tr("Rollovers") << tr("Used Stoped") << tr("Save") << tr("Tested");


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

        ui->tableWidget->setCellWidget(i + 1,7,saveButtons[i]);
        ui->tableWidget->setCellWidget(i + 1,8,testButtons[i]);
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

    ui->tableWidget->setCellWidget(2,6,reserveButtons[0]);
    ui->tableWidget->setCellWidget(4,6,reserveButtons[1]);

    QStringList ql;
    ql << tr("first") << tr("reserve") << tr("second") << tr("reserve") << tr("third");
    setItemNames(ql);
    for(int j =0; j< MAX_POINTS + 1;j++){
        for(int i=0;i <COLUMN_COUNTS ; i++){
            if(!ui->tableWidget->cellWidget(j,i) && !ui->tableWidget->item(j,i)){
                QTableWidgetItem * item = new QTableWidgetItem();
                item->setText("");
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->tableWidget->setItem(j,i,item);            }
        }
    }

//    itemClicked ( QTableWidgetItem * item ) [signal]
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(itemClicked(QTableWidgetItem*)));

//    connect(ui->tableWidget,SIGNAL(itemPressed(QTableWidgetItem*)),
//            this,SLOT(itemClicked(QTableWidgetItem*)));
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

ICProgramPage::~ICProgramPage()
{
    delete ui;
}

void ICProgramPage::showEvent(QShowEvent *e)
{
    int flag = ICParametersSave::Instance()->ProgramInnerFlag();
    for(int i=0;i<MAX_POINTS - 3;i++){
        reserveButtons[i]->setChecked(flag & ( 1 << (_index * 2 +  i)));
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

}

void ICProgramPage::testButonsClicked()
{

}

void ICProgramPage::usedButtonsClicked(bool checked)
{
    ICCheckedButton *button = qobject_cast<ICCheckedButton*>(sender());
    int index = reserveButtons.indexOf(button);
    int flag = ICParametersSave::Instance()->ProgramInnerFlag();
    if(checked){
        flag |= 1 <<  (_index * 2 +  index);
    }
    else{
        flag &= ~(1 <<  (_index * 2 +  index));
    }
    ICParametersSave::Instance()->SetProgramInnerFlag(flag);
}



void ICProgramPage::on_pushButton_clicked()
{
    emit returnProgram();
}
