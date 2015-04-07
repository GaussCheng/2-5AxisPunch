#include "icautoactionpage.h"
#include "ui_icautoactionpage.h"

ICAutoActionPage::ICAutoActionPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICAutoActionPage)
{
    ui->setupUi(this);
}

ICAutoActionPage::~ICAutoActionPage()
{
    delete ui;
}
