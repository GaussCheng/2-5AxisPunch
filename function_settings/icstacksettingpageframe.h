#ifndef ICSTACKSETTINGPAGEFRAME_H
#define ICSTACKSETTINGPAGEFRAME_H

#include <QFrame>

namespace Ui {
    class ICStackSettingPageFrame;
}

class ICStackSettingPageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICStackSettingPageFrame(QWidget *parent = 0);
    ~ICStackSettingPageFrame();

private:
    Ui::ICStackSettingPageFrame *ui;
};

#endif // ICSTACKSETTINGPAGEFRAME_H
