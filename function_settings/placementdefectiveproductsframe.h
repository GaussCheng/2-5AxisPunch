#ifndef PLACEMENTDEFECTIVEPRODUCTSFRAME_H
#define PLACEMENTDEFECTIVEPRODUCTSFRAME_H

#include <QFrame>

namespace Ui {
    class PlacementDefectiveProductsFrame;
}

class PlacementDefectiveProductsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlacementDefectiveProductsFrame(QWidget *parent = 0);
    ~PlacementDefectiveProductsFrame();

private:
    Ui::PlacementDefectiveProductsFrame *ui;
};

#endif // PLACEMENTDEFECTIVEPRODUCTSFRAME_H
