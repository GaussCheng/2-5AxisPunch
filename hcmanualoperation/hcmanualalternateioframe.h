#ifndef HCMANUALALTERNATEIOFRAME_H
#define HCMANUALALTERNATEIOFRAME_H

#include <QFrame>

namespace Ui {
    class HCManualAlternateIOFrame;
}

class HCManualAlternateIOFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HCManualAlternateIOFrame(QWidget *parent = 0);
    ~HCManualAlternateIOFrame();

private:
    Ui::HCManualAlternateIOFrame *ui;
};

#endif // HCMANUALALTERNATEIOFRAME_H
