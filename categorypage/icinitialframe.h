#ifndef ICINITIALFRAME_H
#define ICINITIALFRAME_H

#include "icframe.h"

class ICLabelWrapper;

namespace Ui {
    class ICInitialFrame;
}

class ICInitialFrame : public ICFrame
{
    Q_OBJECT

public:
    explicit ICInitialFrame(QWidget *parent = 0);
    ~ICInitialFrame();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
//    void SetVisibleStatus(bool visibleStatus);

private:
    Ui::ICInitialFrame *ui;

private slots:
//    void on_pushButton_3_clicked();
//    void on_pushButton_4_clicked();
//    void on_pushButton_2_clicked();
//    void on_pushButton_clicked();

private slots:
};

#endif // ICINITIALFRAME_H
