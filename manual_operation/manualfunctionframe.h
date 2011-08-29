#ifndef MANUALFUNCTIONFRAME_H
#define MANUALFUNCTIONFRAME_H

#include <QFrame>

namespace Ui {
    class ManualFunctionFrame;
}

class ManualFunctionFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ManualFunctionFrame(QWidget *parent = 0);
    ~ManualFunctionFrame();

private:
    Ui::ManualFunctionFrame *ui;
};

#endif // MANUALFUNCTIONFRAME_H
