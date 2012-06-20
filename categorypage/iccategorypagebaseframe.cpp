#include "iccategorypagebaseframe.h"
#include "ui_iccategorypagebaseframe.h"

#include <QTimer>
#include <QDialog>

#include <QDebug>

ICCategoryPageBaseFrame::ICCategoryPageBaseFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICCategoryPageBaseFrame),
    showWindowTimer_(NULL)
{
    ui->setupUi(this);

//    ui->funcShortcutsToolButton->move(800 - ui->funcShortcutsToolButton->width(), 0);

    InitSignal();
}

ICCategoryPageBaseFrame::~ICCategoryPageBaseFrame()
{
    delete ui;
}

void ICCategoryPageBaseFrame::InitSignal()
{
//    showWindowTimer_ = new QTimer;
//    showWindowTimer_->start(100);

//    connect(showWindowTimer_,
//            SIGNAL(timeout()),
//            this,
//            SLOT(MoveShortCutFrame()));
}


void ICCategoryPageBaseFrame::MoveShortCutFrame()
{
    qDebug() << "Show ShortCut";
}
