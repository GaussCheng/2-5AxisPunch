#include "icpointtype.h"
#include "ui_icpointtype.h"
#include "icparameterconversion.h"

ICPointType* ICPointType::instance_;


ICPointType::ICPointType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICPointType)
{
    ui->setupUi(this);
    Init_();

    boxToType.insert(ui->checkbox_1,OUYY37_ON);
    boxToType.insert(ui->checkbox_2,OUYY37_OFF);
    boxToType.insert(ui->checkbox_3,OUYY40_ON);
    boxToType.insert(ui->checkbox_4,OUYY40_OFF);
    boxToType.insert(ui->checkbox_5,OUYY22_ON);
    boxToType.insert(ui->checkbox_6,OUYY22_OFF);
    boxToType.insert(ui->checkbox_7,OUYY23_ON);
    boxToType.insert(ui->checkbox_8,OUYY23_OFF);
    boxToType.insert(ui->checkbox_9,WAIT_X41);
    boxToType.insert(ui->checkbox_10,WAIT_X42);
    boxToType.insert(ui->checkbox_11,WAIT_X43);
    boxToType.insert(ui->checkbox_12,WAIT_X44);
    boxToType.insert(ui->checkbox_13,RESEARVE);
    boxToType.insert(ui->checkbox_14,SMOOTH);

    foreach(QCheckBox *box,boxToType.keys()){
        box->setText(propertyToStr.value(boxToType.value(box)));
        connect(box,SIGNAL(stateChanged(int)),
                SLOT(stateChanged(int)));

    }
    QIntValidator * validator = new QIntValidator(0, 30000, this);
    ui->delayEdit->SetDecimalPlaces(2);
    ui->delayEdit->setValidator(validator);
    ui->delayEdit->SetThisIntToThisText(0);

}

void ICPointType::Init_()
{
    typeToStr.insert(Get_Wait,tr("Get_Wait"));
    typeToStr.insert(Get_Wait2,tr("Get_Wait"));
    typeToStr.insert(Get_Wait3,tr("Get_Wait"));
    typeToStr.insert(Get_Up,tr("Get_Up"));
    typeToStr.insert(Get,tr("Get"));
    typeToStr.insert(Put_Wait,tr("Put_Wait"));
    typeToStr.insert(Put_Wait2,tr("Put_Wait"));
    typeToStr.insert(Put_Up,tr("Put_Up"));
    typeToStr.insert(Put,tr("Put"));
    typeToStr.insert(Reserve,tr("Reserve"));

    propertyToStr.insert(OUYY37_ON,tr("OUYY37_ON"));
    propertyToStr.insert(OUYY37_OFF,tr("OUYY37_OFF"));
    propertyToStr.insert(OUYY40_ON,tr("OUYY40_ON"));
    propertyToStr.insert(OUYY40_OFF,tr("OUYY40_OFF"));
    propertyToStr.insert(OUYY22_ON,tr("OUYY22_ON"));
    propertyToStr.insert(OUYY22_OFF,tr("OUYY22_OFF"));
    propertyToStr.insert(OUYY23_ON,tr("OUYY23_ON"));
    propertyToStr.insert(OUYY23_OFF,tr("OUYY23_OFF"));
    propertyToStr.insert(WAIT_X41,tr("WAIT_X41"));
    propertyToStr.insert(WAIT_X42,tr("WAIT_X42"));
    propertyToStr.insert(WAIT_X43,tr("WAIT_X43"));
    propertyToStr.insert(WAIT_X44,tr("WAIT_X44"));
    propertyToStr.insert(RESEARVE,tr("NULL_Property"));
    propertyToStr.insert(SMOOTH,tr("SMOOTH"));


}

ICPointType::~ICPointType()
{
    delete ui;
}

QString ICPointType::toString(PointType type)
{
    if(type == NULL_Type)
        return typeToStr.value(Reserve);
    else
        return typeToStr.value(type);
}

QString ICPointType::toString(PointProperty type)
{
    if(type == NULL_Property)
        return propertyToStr.value(NULL_Property);
    else
        return propertyToStr.value(type);
}

QString ICPointType::toPropertyString(PointProperty type,quint32 delay)
{
    if(type == NULL_Property)
        return propertyToStr.value(NULL_Property);
    else
        return propertyToStr.value(type) + QString("%1s").arg(ICParameterConversion::TransThisIntToThisText(delay, 2));
}

ICPointConfig ICPointType::config()
{
    ICPointConfig config(currentPropertyType() == RESEARVE ? Reserve : Point_Property  ,
                         currentPropertyType(),
                         ui->delayEdit->TransThisTextToThisInt());
    return config;
}


PointProperty ICPointType::currentPropertyType()
{
    return boxToType.value(_box,NULL_Property);
}

void ICPointType::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:{
        ui->retranslateUi(this);
        foreach(QCheckBox *box,boxToType.keys()){
             box->setText(propertyToStr.value(boxToType.value(box)));
        }
    }
        break;
    default:
        break;
    }
}

void ICPointType::stateChanged(int status)
{
    if(status){
        _box = qobject_cast<QCheckBox*> (sender());
    }
    if(_box == ui->checkbox_13){
        ui->delayEdit->setEnabled(false);
    }
    else{
        ui->delayEdit->setEnabled(true);
    }
}
