#ifndef PROGRAMMONITORFRAME_H
#define PROGRAMMONITORFRAME_H

#include <QFrame>

namespace Ui {
    class ProgramMonitorFrame;
}

class ProgramMonitorFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ProgramMonitorFrame(QWidget *parent = 0);
    ~ProgramMonitorFrame();

private:
    Ui::ProgramMonitorFrame *ui;
};

#endif // PROGRAMMONITORFRAME_H
