#ifndef ICSPLASHSCREEN_H
#define ICSPLASHSCREEN_H

#include <QSplashScreen>
#include <QProgressBar>
#include <QLabel>

class ICSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit ICSplashScreen(const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0);
    void SetRange(int minum, int maxum) { progressBar_->setRange(minum, maxum);}

protected:
    void mousePressEvent(QMouseEvent *){}
    void mouseReleaseEvent(QMouseEvent *){}
signals:

public slots:
    void SetProgreeBarValue(int value) { progressBar_->setValue(value);}

private slots:
    void OnMessageChanged(const QString &message);
private:
    QProgressBar *progressBar_;
    QLabel* swVersionLabel_;
    QLabel* swVersion_;
    QLabel* hwVersionLabel_;
    QLabel* hwVersion_;
    QLabel* clientLabel_;
    QLabel* client_;
    QLabel* dateLabel_;
    QLabel* date_;
    QWidget* versionContainer_;

};

#endif // ICSPLASHSCREEN_H
