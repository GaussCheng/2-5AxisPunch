#include "icscreensaver.h"
#include "ui_icscreensaver.h"

ICScreenSaver::ICScreenSaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICScreenSaver)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

ICScreenSaver::~ICScreenSaver()
{
    delete ui;
}

void ICScreenSaver::on_pushButton_clicked()
{
    this->hide();
    emit Unlock();
}
