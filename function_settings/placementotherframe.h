#ifndef PLACEMENTOTHERFRAME_H
#define PLACEMENTOTHERFRAME_H

#include <QFrame>

namespace Ui {
    class PlacementOtherFrame;
}

class PlacementOtherFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlacementOtherFrame(QWidget *parent = 0);
    ~PlacementOtherFrame();

private:
    Ui::PlacementOtherFrame *ui;
};

#endif // PLACEMENTOTHERFRAME_H
