#ifndef ICSYSTEMSETTINGFRAME_H
#define ICSYSTEMSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ICSystemSettingFrame;
}

class ICSystemSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICSystemSettingFrame(QWidget *parent = 0);
    ~ICSystemSettingFrame();

private:
    Ui::ICSystemSettingFrame *ui;
};

#endif // ICSYSTEMSETTINGFRAME_H
