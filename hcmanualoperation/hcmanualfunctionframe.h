#ifndef HCMANUALFUNCTIONFRAME_H
#define HCMANUALFUNCTIONFRAME_H

#include <QFrame>

namespace Ui {
    class HCManualFunctionFrame;
}

class HCManualFunctionFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HCManualFunctionFrame(QWidget *parent = 0);
    ~HCManualFunctionFrame();

private:
    Ui::HCManualFunctionFrame *ui;
};

#endif // HCMANUALFUNCTIONFRAME_H
