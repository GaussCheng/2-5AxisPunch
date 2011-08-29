#ifndef ICSPLASHSCREEN_H
#define ICSPLASHSCREEN_H

#include <QSplashScreen>
#include <QProgressBar>

class ICSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit ICSplashScreen(const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0);
    void SetRange(int minum, int maxum) { progressBar_->setRange(minum, maxum);}

signals:

public slots:
    void SetProgreeBarValue(int value) { progressBar_->setValue(value);}

private slots:
    void OnMessageChanged(const QString &message);
private:
    QProgressBar *progressBar_;
};

#endif // ICSPLASHSCREEN_H
