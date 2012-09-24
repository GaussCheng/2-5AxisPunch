#include "icupdatelogodialog.h"
#include "ui_icupdatelogodialog.h"

ICUpdateLogoDialog::ICUpdateLogoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICUpdateLogoDialog)
{
    ui->setupUi(this);
}

ICUpdateLogoDialog::~ICUpdateLogoDialog()
{
    delete ui;
}

void ICUpdateLogoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICUpdateLogoDialog::on_scanPictures_clicked()
{
#ifdef Q_WS_X11
    ui->picView->ScanPictures("./resource");
#else
    ui->picView->ScanPictures("/mnt/udisk/HCUpdate_pic");
#endif
}

void ICUpdateLogoDialog::on_setToStartup_clicked()
{
    startupPage_ = ui->picView->CurrentSelectedPicture();
    ui->startupPage->setText(startupPage_);
}

void ICUpdateLogoDialog::on_setToStandby_clicked()
{
    standbyPage_ = ui->picView->CurrentSelectedPicture();
    ui->standbyPage->setText(standbyPage_);
}

void ICUpdateLogoDialog::on_okButton_clicked()
{
    ::system("rm /opt/Qt/bin/resource/*");
    if(!startupPage_.isEmpty())
    {
#ifdef Q_WS_X11
        QFile::copy(startupPage_, "/home/gausscheng/startup_page.png");
#else
        QFile::copy(startupPage_, "/opt/Qt/bin/resource/startup_page.png");
#endif
    }
    if(!standbyPage_.isEmpty())
    {
#ifdef Q_WS_X11
        QFile::copy(standbyPage_, "/home/gausscheng/Standby.png");
#else
        QFile::copy(standbyPage_, "/opt/Qt/bin/resource/Standby.png");
#endif
    }
    this->accept();
}

void ICUpdateLogoDialog::on_cancelButton_clicked()
{
    this->reject();
}
