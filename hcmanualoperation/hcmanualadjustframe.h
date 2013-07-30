#ifndef HCMANUALADJUSTFRAME_H
#define HCMANUALADJUSTFRAME_H

#include <QWidget>
#include <QSignalMapper>
#include <QBitArray>
#include "iccommandkeywrapper.h"

namespace Ui {
    class HCManualAdjustFrame;
}

class HCManualAdjustFrame : public QWidget
{
    Q_OBJECT

public:
    explicit HCManualAdjustFrame(QWidget *parent = 0);
    void ChangeButtonColor();
    ~HCManualAdjustFrame();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);
private slots:
    void StatusRefreshed();

    void on_AdjustForbidButton_clicked();

private:
    Ui::HCManualAdjustFrame *ui;
//    QSignalMapper signalMapper_;
//    QPixmap plusPic_;
//    QPixmap minsPic_;
//    QPixmap statusOnPix_;
//    QPixmap statusOffPix_;
    QBitArray currentStatus_;
    QList<ICCommandKeyWrapper*> wrappers_;
    int timerID_;
    const QPixmap offPixmap_;
    const QPixmap inputOnPixmap_;
    const QPixmap outputOnPixmap_;
};

#endif // HCMANUALADJUSTFRAME_H
