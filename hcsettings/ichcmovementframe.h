#ifndef ICHCMOVEMENTFRAME_H
#define ICHCMOVEMENTFRAME_H

#include <QFrame>

namespace Ui {
    class ICHCMovementFrame;
}

class ICHCMovementFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCMovementFrame(QWidget *parent = 0);
    ~ICHCMovementFrame();

private:
    Ui::ICHCMovementFrame *ui;
};

#endif // ICHCMOVEMENTFRAME_H
