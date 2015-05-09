#include "icpointpproperty.h"
#include "ui_icpointpproperty.h"



ICPointpProperty::ICPointpProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICPointpProperty)
{
    ui->setupUi(this);
    boxs << ui->checkBox_1 << ui->checkBox_2 << ui->checkBox_3 << ui->checkBox_4
         << ui->checkBox_5 << ui->checkBox_6 << ui->checkBox_7 << ui->checkBox_8
         << ui->checkBox_9;

    boxToproperty.insert(ui->checkBox_1,PointProperty1);
    boxToproperty.insert(ui->checkBox_2,PointProperty2);
    boxToproperty.insert(ui->checkBox_3,PointProperty3);
    boxToproperty.insert(ui->checkBox_4,PointProperty4);
    boxToproperty.insert(ui->checkBox_5,PointProperty5);
    boxToproperty.insert(ui->checkBox_6,PointProperty6);
    boxToproperty.insert(ui->checkBox_7,PointProperty7);
    boxToproperty.insert(ui->checkBox_8,PointProperty8);
    boxToproperty.insert(ui->checkBox_9,PointProperty9);



    for(int i=USE_PROPERTY;i<boxs.size();i++)
        boxs.at(i)->hide();

}

ICPointpProperty::~ICPointpProperty()
{
    delete ui;
}

quint32 ICPointpProperty::GetPointProperty()
{
    quint32 propertys = 0;
    for(int i=0;i<USE_PROPERTY;i++){
        propertys |= (boxs.at(i)->isChecked() << i);
    }
    return propertys;
}

void ICPointpProperty::SetPointProperty(quint32 p)
{
    for(int i=0;i<USE_PROPERTY;i++){
        boxs.at(i)->setChecked(p & (1 << i));
    }
}
