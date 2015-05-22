#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "icscreensavergui.h"
#include "icutility.h"

ICScreenSaverGUI::ICScreenSaverGUI(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    backgroundLabel_ = new QLabel(this);
    unlockButton_ = new QPushButton(tr("Unlock"), this);
    showFullScreen();
    const QSize screenSize = qApp->desktop()->size();
    QPixmap img(ICUtility::ScreenSaverImgPath);
    if(!img.isNull())
    {
        img = img.scaled(screenSize);
        backgroundLabel_->resize(screenSize);
        backgroundLabel_->move(0, 0);
        backgroundLabel_->setPixmap(img);
    }
    const int height = screenSize.height();
    unlockButton_->move(screenSize.width() >> 1, height - (height >> 3) );
    connect(unlockButton_,
            SIGNAL(clicked()),
            this,
            SLOT(close()));

}

ICScreenSaverGUI::~ICScreenSaverGUI()
{
    delete backgroundLabel_;
    delete unlockButton_;
}
