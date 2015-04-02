#include "icprogrammainpage.h"
#include "ui_icprogrammainpage.h"
#include "icparameterssave.h"
#include "icmold.h"
#include "icfile.h"

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

    QStringList items;
    items << tr("Get Wait") << tr("reserve") << tr("Get Up") << tr("reserve") << tr("Get");
    programPages[0]->setItemNames(items);
    items.clear();
    items << tr("Put Wait") << tr("reserve") << tr("Put Up") << tr("reserve")  << tr("Put");
    programPages[1]->setItemNames(items);



     QStringList ql;
     ql << tr("Mould Get") << tr("Mould Put") << tr("Reserve") << tr("Reserve")
           << tr("Reserve") << tr("Reserve") << tr("Reserve") << tr("Reserve");
     setVerticalNames(ql);
     oldUsed  = ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(ICMold::programInnerUsed));

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
    int used = ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(ICMold::programUsed));
    for(int i=0;i < MAX_ROWS;i++){
            usedButtons[i]->setChecked(used & ( 1 << i));
            programButtons[i]->setEnabled(used & ( 1 << i));
    }

    ui->stackedWidget->setCurrentWidget(ui->mainPage);
    QWidget::showEvent(e);
}

void ICProgramMainPage::hideEvent(QHideEvent *e)
{
    if(MoldChanged()){
        qDebug() << "SaveProgramToFiles: " << SaveProgramToFiles(GT_AllItems());
    }
    QWidget::hideEvent(e);
}

void ICProgramMainPage::usedButtonClicked(bool checked)
{
    ICCheckedButton *button = qobject_cast<ICCheckedButton*>(sender());
    int index = usedButtons.indexOf(button);
    programButtons.at(index)->setEnabled(checked);
    int used = ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(ICMold::programUsed));
    if(checked){
        used |= ( 1 << index);
    }
    else{
        used &= ~( 1 << index);
    }
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(ICMold::programUsed), used);
    ICMold::CurrentMold()->SaveMoldParamsFile();

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

QList<MoldItemPtr> ICProgramMainPage::GT_AllItems()
{
    return GT_CalculateItem(programPages[0]->GT_Items()  +  programPages[1]->GT_Items());
}

bool ICProgramMainPage::SaveProgramToFiles(QList<MoldItemPtr> items)
{
    bool ret = false;
    MoldReSum(items);
    QByteArray toWrite;

    for(int i = 0; i != items.size(); ++i)
    {
        toWrite += items.at(i)->ToString() + "\n";
    }
    ICFile file(ICMold::CurrentMold()->MoldName()+"_bak");
    ret = file.ICWrite(toWrite);
    return ret;
}

void ICProgramMainPage::MoldReSum(QList<MoldItemPtr> items)
{
    int seq = 0;
    for(int i = 0; i != items.size(); ++i)
    {
        items[i]->SetSeq(seq);
        items[i]->ReSum();
        ++seq;
    }
}

bool ICProgramMainPage::MoldChanged()
{
    if(oldUsed != _MoldParam(ICMold::programInnerUsed) ){
        oldUsed =_MoldParam(ICMold::programInnerUsed);
        return true;
    }
}

QList<MoldItemPtr> ICProgramMainPage::GT_CalculateItem(QList<MoldItemPtr> items)
{
    int oldNum = 0;
    //计算num
    for(int i=0;i<items.size();i++){
        if(items.at(i)->GMVal() == ICMold::GARC){
            items.at(i)->SetNum(oldNum);
            if(items.at(i)->IFPos()  == 5){
                oldNum ++;
            }
        }
        else{
            items.at(i)->SetNum(oldNum);
            oldNum ++;
        }
    }


   return items;
}
