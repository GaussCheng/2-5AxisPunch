#ifndef ICSECURITYPOINTSETTINGFRAME_H
#define ICSECURITYPOINTSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ICSecurityPointSettingFrame;
}

class ICSecurityPointSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICSecurityPointSettingFrame(QWidget *parent = 0);
    ~ICSecurityPointSettingFrame();

private:
    Ui::ICSecurityPointSettingFrame *ui;
};

#endif // ICSECURITYPOINTSETTINGFRAME_H
