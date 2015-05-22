#ifndef ICSPLASHSCREEN_H
#define ICSPLASHSCREEN_H

#include <QSplashScreen>
#include <QProgressBar>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit ICSplashScreen(const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0);
    ~ICSplashScreen();
    void SetRange(int minum, int maxum) { progressBar_->setRange(minum, maxum);}

    QFont TextFont() const { return font_;}
    void SetTextFont(const QFont& font);

signals:

public slots:
    void SetProgreeBarValue(int value) { progressBar_->setValue(value);}

private slots:
    void OnMessageChanged(const QString &message);
private:
    QFont font_;
    QProgressBar *progressBar_;
};

inline void ICSplashScreen::SetTextFont(const QFont &font)
{
    font_ = font;
    font_.setPixelSize((font_.pointSize() << 2) / 3);
    this->setFont(font_);
}
#endif // ICSPLASHSCREEN_H
