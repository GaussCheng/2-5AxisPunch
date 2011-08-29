#ifndef MANUALALTERNATEIOFRAME_H
#define MANUALALTERNATEIOFRAME_H

#include <QFrame>

namespace Ui {
    class ManualAlternateIOFrame;
}

class ManualAlternateIOFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ManualAlternateIOFrame(QWidget *parent = 0);
    ~ManualAlternateIOFrame();

private:
    Ui::ManualAlternateIOFrame *ui;
};

#endif // MANUALALTERNATEIOFRAME_H
