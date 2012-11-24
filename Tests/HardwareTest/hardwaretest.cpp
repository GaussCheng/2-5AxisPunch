#include "hardwaretest.h"
#include "ui_hardwaretest.h"
#include "testkeyboardpage.h"
#include "testlcdcolorpage.h"
#include "testbrightnesspage.h"
#include "testbacklightpage.h"
#include "testsoundpage.h"
#include "testtouchscreenpage.h"
#include "testledpage.h"
#include "testcommunicatepage.h"


HardwareTest::HardwareTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareTest)
{
    ui->setupUi(this);
    keyboardWidget_ = new TestKeyboardPage();
    colorWidget_ = new TestLCDColorPage(ui->centerWidget);
    brightnessWidget_ = new TestBrightnessPage();
    backlightWidget_ = new TestBacklightPage();
    soundWidget_ = new TestSoundPage();
    touchScreenWidget_ = new TestTouchScreenPage();
    testLedWidget_ = new TestLedPage();
    communicateWidget_ = new TestCommunicatePage();
    reportPage_ = new TestReportPage();
    AddTestPageHelper_(keyboardWidget_);
    AddTestPageHelper_(colorWidget_);
    AddTestPageHelper_(brightnessWidget_);
    AddTestPageHelper_(backlightWidget_);
    AddTestPageHelper_(soundWidget_);
    AddTestPageHelper_(touchScreenWidget_);
    AddTestPageHelper_(testLedWidget_);
    AddTestPageHelper_(communicateWidget_);
    ui->centerWidget->addWidget(reportPage_);
    currentTest_ = 1;
    showFullScreen();
}

HardwareTest::~HardwareTest()
{
    delete ui;
}

void HardwareTest::changeEvent(QEvent *e)
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

void HardwareTest::OnSingleTestFinish()
{
    ui->centerWidget->setCurrentIndex(currentTest_++);
    if(ui->centerWidget->currentWidget() == reportPage_)
    {
        reportPage_->SetKeyTestResult(keyboardWidget_->TestDescription());
        reportPage_->SetColorTestResult(colorWidget_->TestDescription());
        reportPage_->SetBrightnessTestResult(brightnessWidget_->TestDescription());
        reportPage_->SetBacklightResult(backlightWidget_->TestDescription());
        reportPage_->SetSoundTestResult(soundWidget_->TestDescription());
        reportPage_->SetTouchScreenTestResult(touchScreenWidget_->TestDescription());
        reportPage_->SetLEDTestResult(testLedWidget_->TestDescription());
        reportPage_->SetCommunicateTestResult(communicateWidget_->TestDescription());
    }
}

void HardwareTest::AddTestPageHelper_(TestPageBase *page)
{
    ui->centerWidget->addWidget(page);
    connect(page,
            SIGNAL(TestFinished()),
            SLOT(OnSingleTestFinish()));
}
