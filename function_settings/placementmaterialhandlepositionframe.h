#ifndef PLACEMENTMATERIALHANDLEPOSITIONFRAME_H
#define PLACEMENTMATERIALHANDLEPOSITIONFRAME_H

#include <QFrame>

namespace Ui {
    class PlacementMaterialHandlePositionFrame;
}

class PlacementMaterialHandlePositionFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlacementMaterialHandlePositionFrame(QWidget *parent = 0);
    ~PlacementMaterialHandlePositionFrame();

private:
    Ui::PlacementMaterialHandlePositionFrame *ui;
};

#endif // PLACEMENTMATERIALHANDLEPOSITIONFRAME_H
