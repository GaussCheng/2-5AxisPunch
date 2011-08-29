#include "icproductsettingframe.h"
#include "ui_icproductsettingframe.h"

ICProductSettingFrame::ICProductSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICProductSettingFrame)
{
    ui->setupUi(this);
}

ICProductSettingFrame::~ICProductSettingFrame()
{
    delete ui;
}
