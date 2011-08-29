#include "icsplashscreen.h"

ICSplashScreen::ICSplashScreen(const QPixmap &pixmap, Qt::WindowFlags f) :
    QSplashScreen(pixmap, f)
{
    progressBar_ = new QProgressBar(this);
    progressBar_->resize(this->width() - 200, 20);
    progressBar_->move(100, height() * 7 / 8 );
    connect(this,
            SIGNAL(messageChanged(QString)),
            this,
            SLOT(OnMessageChanged(QString)));
}

void ICSplashScreen::OnMessageChanged(const QString &message)
{
    Q_UNUSED(message)
    progressBar_->setValue(progressBar_->value() + 1);
}
