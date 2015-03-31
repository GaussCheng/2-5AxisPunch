#include "icprogrammainpage.h"
#include "ui_icprogrammainpage.h"
#include "icparameterssave.h"



ICProgramMainPage::ICProgramMainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICProgramMainPage)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(MAX_ROWS);
    ui->tableWidget->setColumnCount(3);

    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    for(int i=0;i<ui->tableWidget->rowCount();i++){
        ui->tableWidget->setVerticalHeaderItem(i,new QTableWidgetItem(QString("S%1").arg(i+1)));
    }

    for(int i=0;i < MAX_ROWS;i++){
        usedButtons.append(new ICCheckedButton);
        usedButtons[i]->setName(tr("Skiped"));
        usedButtons[i]->setCheckedName(tr("Used"));
        usedButtons[i]->setFocusPolicy(Qt::NoFocus);
        usedButtons[i]->setMinimumHeight(40);
        connect(usedButtons[i],SIGNAL(clicked(bool)),
                SLOT(usedButtonClicked(bool)));

        programButtons.append(new QPushButton);
        programButtons[i]->setFocusPolicy(Qt::NoFocus);
        programButtons[i]->setEnabled(false);
        connect(programButtons[i],SIGNAL(clicked()),
                SLOT(programButtonClicked()));

        programButtons[i]->setText(tr("Jump Program"));
        programButtons[i]->setMinimumHeight(40);

        ui->tableWidget->setCellWidget(i,1,usedButtons[i]);
        ui->tableWidget->setCellWidget(i,2,programButtons[i]);

        ICProgramPage * page = new ICProgramPage(this,i);
        programPages.append(page);
        ui->stackedWidget->addWidget(page);
        connect(page,SIGNAL(returnProgram()),
                SLOT(showMainProgram()));
    }


     QStringList ql;
     ql << tr("Mould Get") << tr("Mould Put") << tr("Reserve") << tr("Reserve")
           << tr("Reserve") << tr("Reserve") << tr("Reserve") << tr("Reserve");
     setVerticalNames(ql);

}

void ICProgramMainPage::setVerticalNames(QStringList &contents)
{
    Q_ASSERT(contents.size() == MAX_ROWS);
    for(int i = 0 ;i < MAX_ROWS;i++){
        if(ui->tableWidget->item(i,0)){
            ui->tableWidget->item(i,0)->setText(contents.at(i));

        }
        else{
            QTableWidgetItem * item = new QTableWidgetItem(contents.at(i));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->setItem(i,0,item);
        }
    }
}

ICProgramMainPage::~ICProgramMainPage()
{
    delete ui;
}

void ICProgramMainPage::showEvent(QShowEvent *e)
{
    int program = ICParametersSave::Instance()->ProgramUsedFlag();
    for(int i=0;i < MAX_ROWS;i++){
            usedButtons[i]->setChecked(program & ( 1 << i));
            programButtons[i]->setEnabled(program & ( 1 << i));
    }

    ui->stackedWidget->setCurrentWidget(ui->mainPage);
    QWidget::showEvent(e);
}

void ICProgramMainPage::usedButtonClicked(bool checked)
{
    ICCheckedButton *button = qobject_cast<ICCheckedButton*>(sender());
    int index = usedButtons.indexOf(button);
    programButtons.at(index)->setEnabled(checked);
    int flag = ICParametersSave::Instance()->ProgramUsedFlag();
    if(checked){
        flag |= ( 1 << index);
    }
    else{
        flag &= ~( 1 << index);
    }
    ICParametersSave::Instance()->SetProgramUsedFlag(flag);
}

void ICProgramMainPage::programButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int index = programButtons.indexOf(button);
    ui->stackedWidget->setCurrentWidget(programPages.at(index));


}

void ICProgramMainPage::showMainProgram()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}
