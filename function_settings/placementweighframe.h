#ifndef PLACEMENTWEIGHFRAME_H
#define PLACEMENTWEIGHFRAME_H

#include <QFrame>

namespace Ui {
    class PlacementWeighFrame;
}

class PlacementWeighFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlacementWeighFrame(QWidget *parent = 0);
    ~PlacementWeighFrame();

private:
    Ui::PlacementWeighFrame *ui;
};

#endif // PLACEMENTWEIGHFRAME_H
