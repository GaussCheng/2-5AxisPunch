#include "icfunctionpagebackframe.h"
#include "ui_icfunctionpagebackframe.h"

#include "icfunctionpageframe.h"

ICFunctionPageBackFrame::ICFunctionPageBackFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICFunctionPageBackFrame),
    pageLayout_(new QStackedLayout)
{
    ui->setupUi(this);

    this->setLayout(pageLayout_);

    if(pageLayout_ != NULL)
    {
        functionPage_ = new ICFunctionPageFrame(this, pageLayout_);
        pageLayout_->addWidget(functionPage_);
        pageLayout_->setCurrentWidget(functionPage_);
        connect(functionPage_,
                SIGNAL(SettingPageShown(QString)),
                SIGNAL(SettingPageShown(QString)));
    }
}

ICFunctionPageBackFrame::~ICFunctionPageBackFrame()
{
    delete ui;
}

void ICFunctionPageBackFrame::ShowFunctionSelectPage()
{
    functionPage_->ShowFunctionSelectPage();
}

