#include "testreportpage.h"
#include "ui_testreportpage.h"

TestReportPage::TestReportPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestReportPage)
{
    ui->setupUi(this);
    ui->commWidget->SetTestTimes(9999999);
}

TestReportPage::~TestReportPage()
{
    delete ui;
}

void TestReportPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TestReportPage::on_restartButton_clicked()
{
    ::system("reboot");
}

void TestReportPage::SetKeyTestResult(const QString &desc)
{
    ui->key->setText(desc);
}

void TestReportPage::SetColorTestResult(const QString &desc)
{
    ui->color->setText(desc);
}

void TestReportPage::SetBrightnessTestResult(const QString &desc)
{
    ui->brightness->setText(desc);
}

void TestReportPage::SetSoundTestResult(const QString &desc)
{
    ui->sound->setText(desc);
}

void TestReportPage::SetTouchScreenTestResult(const QString &desc)
{
    ui->touch->setText(desc);
}

void TestReportPage::SetLEDTestResult(const QString &desc)
{
    ui->led->setText(desc);
}

void TestReportPage::SetCommunicateTestResult(const QString &desc)
{
    ui->communicate->setText(desc);
}

void TestReportPage::SetBacklightResult(const QString &desc)
{
    ui->backlight->setText(desc);
}
