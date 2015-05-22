#ifndef ICINJECTIONMACHINEWIDGET_H
#define ICINJECTIONMACHINEWIDGET_H

#include <QGraphicsView>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICInjectionMachineWidget : public QGraphicsView
{
    Q_OBJECT
public:
    enum ScrewBaseDirection{ ScrewBaseDirection_Front, ScrewBaseDirection_Back};
    explicit ICInjectionMachineWidget(QWidget *parent = 0);
    virtual ~ICInjectionMachineWidget();
    void SetScrewBasePos(qreal pos);
    void SetScrewPos(qreal pos);
    void SetEjectorPos(qreal pos);
    void SetEjectorBasePos(qreal pos);
    void SetScrewBaseDirection(ScrewBaseDirection direction) { direction_ = direction;}

    void SetScrewDistance(qreal length);
    void SetScrewBaseDistance(qreal length);
    void SetEjectorDistance(qreal length);
    void SetEjectorBaseDistance(qreal length);

signals:

public slots:

private:
    QGraphicsScene* scene_;
    QGraphicsPixmapItem* machineDesk_;
    QGraphicsPixmapItem* machineEjectorBase_;
    QGraphicsPixmapItem* machineEjector_;
    QGraphicsPixmapItem* machineScrew_;
    QGraphicsItemGroup* machineScrewBase_;

    qreal screwFactor_;
    qreal screwBaseFactor_;
    qreal ejectorFactor_;
    qreal ejectorBaseFactor_;
    ScrewBaseDirection direction_;
};

inline void ICInjectionMachineWidget::SetScrewDistance(qreal length)
{
    screwFactor_ = 35 / (length + 1);
}

inline void ICInjectionMachineWidget::SetScrewBaseDistance(qreal length)
{
    screwBaseFactor_ = 35 / (length + 1);
}

inline void ICInjectionMachineWidget::SetEjectorDistance(qreal length)
{
    ejectorFactor_ = 20 / (length + 1);
}

inline void ICInjectionMachineWidget::SetEjectorBaseDistance(qreal length)
{
    ejectorBaseFactor_ = 44 / (length + 1);
}

inline void ICInjectionMachineWidget::SetScrewPos(qreal pos)
{
    qreal s = pos * screwFactor_;
    if(s > 35)
    {
        s = 35;
    }
    machineScrew_->setX(s);
}

inline void ICInjectionMachineWidget::SetEjectorPos(qreal pos)
{
    qreal s = -10 + pos * ejectorBaseFactor_;
    if(s > 10)
    {
        s = 10;
    }
    machineEjector_->setX(s);
}

inline void ICInjectionMachineWidget::SetEjectorBasePos(qreal pos)
{
    qreal s = 180 -  (pos * ejectorBaseFactor_);
    if(s < 135)
    {
        s = 135;
    }
    machineEjectorBase_->setX(s);
}

inline void ICInjectionMachineWidget::SetScrewBasePos(qreal pos)
{
    if(direction_ == ScrewBaseDirection_Front)
    {
        machineScrewBase_->setX((pos * screwBaseFactor_) - 10);
    }
    else
    {
        qreal s = 270 - (pos * screwBaseFactor_);
        if(s < 235)
        {
            s = 235;
        }
        machineScrewBase_->setX(s);
    }
}

#endif // ICINJECTIONMACHINEWIDGET_H
