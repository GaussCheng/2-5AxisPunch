#include "icinjectionmachinewidget.h"
#include <QDebug>

ICInjectionMachineWidget::ICInjectionMachineWidget(QWidget *parent) :
    QGraphicsView(parent),
    direction_(ScrewBaseDirection_Front)
{
//    setOptimizationFlag(QGraphicsView::DontSavePainterState);
//    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    scene_ = new QGraphicsScene();
    QGraphicsPixmapItem* ef = scene_->addPixmap(QPixmap(":/resource/machine_desk_ef.png"));
    machineScrew_ = scene_->addPixmap(QPixmap(":/resource/machine_screw.png"));
    machineEjector_ = scene_->addPixmap(QPixmap(":/resource/machine_ejector.png"));
    QGraphicsPixmapItem* sb = scene_->addPixmap(QPixmap(":/resource/machine_screw_base.png"));
    machineDesk_ = scene_->addPixmap(QPixmap(":/resource/machine_desk.png"));
    machineScrewBase_ = new QGraphicsItemGroup();
    machineEjectorBase_ = scene_->addPixmap(QPixmap(":/resource/machine_ejector_base.png"));
    scene_->addItem(machineScrewBase_);
    this->setScene(scene_);
//    this->scale(1.2, 1.1);
//    machineScrew_->setParentItem(machineScrewBase_);
    machineScrewBase_->addToGroup(machineScrew_);
    machineScrewBase_->addToGroup(sb);
    machineEjector_->setParentItem(machineEjectorBase_);
//    machineEjectorBase_->setPos(16, 30);
//    machineEjector_->setPos(-15, 33);
//    machineScrewBase_->setX(500);
//    machineScrewBase_->setY(-2);
//    ef->setPos(245, 84);
//    ef->setZValue(7);
    machineEjectorBase_->setZValue(6);
    machineDesk_->setZValue(5);
    machineScrewBase_->setZValue(4);
//    sb->setZValue(3);
//    machineScrew_->setZValue(1);
//    machineScrew_->setX(40);
//    moldGroup_ = scene_->createItemGroup(QList<QGraphicsItem*>()<<machineEjectorBase_<<machineEjector_);
//    screwGroup_ = scene_->createItemGroup(QList<QGraphicsItem*>()<<0machineScrewBase_<<machineScrew_);
}

ICInjectionMachineWidget::~ICInjectionMachineWidget()
{
    delete scene_;
}
