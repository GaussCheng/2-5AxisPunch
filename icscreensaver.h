#ifndef ICSCREENSAVER_H
#define ICSCREENSAVER_H

#include <QWidget>

namespace Ui {
    class ICScreenSaver;
}

class ICScreenSaver : public QWidget
{
    Q_OBJECT

public:
    explicit ICScreenSaver(QWidget *parent = 0);
    ~ICScreenSaver();

Q_SIGNALS:
    void Unlock();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ICScreenSaver *ui;
};

#endif // ICSCREENSAVER_H
