#ifndef PLACEMENTQUALITYCONTROLFRAME_H
#define PLACEMENTQUALITYCONTROLFRAME_H

#include <QFrame>

namespace Ui {
    class PlacementQualityControlFrame;
}

class PlacementQualityControlFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlacementQualityControlFrame(QWidget *parent = 0);
    ~PlacementQualityControlFrame();

private:
    Ui::PlacementQualityControlFrame *ui;
};

#endif // PLACEMENTQUALITYCONTROLFRAME_H
