#ifndef PLACEMENTFINISHEDFRAME_H
#define PLACEMENTFINISHEDFRAME_H

#include <QFrame>

namespace Ui {
    class PlacementFinishedFrame;
}

class PlacementFinishedFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlacementFinishedFrame(QWidget *parent = 0);
    ~PlacementFinishedFrame();

private:
    Ui::PlacementFinishedFrame *ui;
};

#endif // PLACEMENTFINISHEDFRAME_H
