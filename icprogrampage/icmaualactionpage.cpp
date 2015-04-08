#include "icmaualactionpage.h"
#include "ui_icmaualactionpage.h"

ICMaualActionPage::ICMaualActionPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICMaualActionPage)
{
    ui->setupUi(this);
}

ICMaualActionPage::~ICMaualActionPage()
{
    delete ui;
}
