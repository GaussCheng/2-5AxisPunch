#ifndef ICSIGNALSETTINGFRAME_H
#define ICSIGNALSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ICSignalSettingFrame;
}

class ICSignalSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICSignalSettingFrame(QWidget *parent = 0);
    ~ICSignalSettingFrame();

private:
    Ui::ICSignalSettingFrame *ui;
};

#endif // ICSIGNALSETTINGFRAME_H
