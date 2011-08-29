#ifndef MANUALFIXTUREFRAME_H
#define MANUALFIXTUREFRAME_H

#include <QFrame>

namespace Ui {
    class ManualFixtureFrame;
}

class ManualFixtureFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ManualFixtureFrame(QWidget *parent = 0);
    ~ManualFixtureFrame();

private:
    Ui::ManualFixtureFrame *ui;
};

#endif // MANUALFIXTUREFRAME_H
