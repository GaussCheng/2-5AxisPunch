#include "routesettingdialog.h"
#include "ui_routesettingdialog.h"
#include "icvirtualhost.h"
#include "iccommandprocessor.h"

RouteSettingDialog::RouteSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RouteSettingDialog)
{
    ui->setupUi(this);

    InitInterface();
}

RouteSettingDialog::~RouteSettingDialog()
{
    delete ui;
}

void RouteSettingDialog::on_okToolButton_clicked()
{
    ICCommandProcessor::Instance()->TeachParam(ICVirtualHost::ST_ARC_TRACK, ui->optionsTableWidget->currentRow() + 1);
    this->accept();
}

void RouteSettingDialog::on_cancelToolButton_clicked()
{
    this->accept();
}

void RouteSettingDialog::InitInterface()
{
    QIntValidator * validator = new QIntValidator(0, 32767, this);
    ui->radiusLineEdit->SetDecimalPlaces(1);
    ui->radiusLineEdit->setValidator(validator);
}

void RouteSettingDialog::on_radiusLineEdit_textChanged(QString )
{
    ICCommandProcessor::Instance()->TeachParam(ICVirtualHost::ST_ARC_RADIO, ui->radiusLineEdit->TransThisTextToThisInt());
}
