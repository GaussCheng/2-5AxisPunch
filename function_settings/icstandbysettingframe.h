#ifndef ICSTANDBYSETTINGFRAME_H
#define ICSTANDBYSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ICStandbySettingFrame;
}

class ICStandbySettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICStandbySettingFrame(QWidget *parent = 0);
    ~ICStandbySettingFrame();

private:
    Ui::ICStandbySettingFrame *ui;
};

#endif // ICSTANDBYSETTINGFRAME_H
