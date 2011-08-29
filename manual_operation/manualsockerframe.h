#ifndef MANUALSOCKERFRAME_H
#define MANUALSOCKERFRAME_H

#include <QFrame>

namespace Ui {
    class ManualSockerFrame;
}

class ManualSockerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ManualSockerFrame(QWidget *parent = 0);
    ~ManualSockerFrame();

private:
    Ui::ManualSockerFrame *ui;
};

#endif // MANUALSOCKERFRAME_H
