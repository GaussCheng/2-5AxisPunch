#include "icpictureview.h"
#include "ui_icpictureview.h"
#include "QMessageBox"


ICPictureView::ICPictureView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICPictureView)
{
    ui->setupUi(this);
}

ICPictureView::~ICPictureView()
{
    delete ui;
}

void ICPictureView::changeEvent(QEvent *e)
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

void ICPictureView::ScanPictures(const QString &path)
{
    picturesDir_.setPath(path);
    QStringList pics = picturesDir_.entryList(QStringList()<<"*.png");
    if(pics.isEmpty())
    {
        QMessageBox::information(this,tr("Tips"),tr("No find picture!"));
        return;
    }
    ui->picList->clear();
    ui->picList->addItems(pics);
}

void ICPictureView::on_picList_itemClicked(QListWidgetItem *item)
{
    currentPic_ = picturesDir_.absoluteFilePath(item->text());
    QPixmap pic(currentPic_);
    ui->picView->setPixmap(pic);
}

QString ICPictureView::CurrentSelectedPicture() const
{
    return currentPic_;
}
