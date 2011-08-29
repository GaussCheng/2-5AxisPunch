#ifndef ICPRODUCTSETTINGFRAME_H
#define ICPRODUCTSETTINGFRAME_H

#include <QFrame>

namespace Ui {
    class ICProductSettingFrame;
}

class ICProductSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICProductSettingFrame(QWidget *parent = 0);
    ~ICProductSettingFrame();

private:
    Ui::ICProductSettingFrame *ui;
};

#endif // ICPRODUCTSETTINGFRAME_H
