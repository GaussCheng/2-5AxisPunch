#ifndef ICHCMANUALOPERATIONPAGEFRAME_H
#define ICHCMANUALOPERATIONPAGEFRAME_H

#include <QFrame>

#include <QMap>
#include <QSignalMapper>
#include "axismodifydialog.h"

namespace Ui {
    class ICHCManualOperationPageFrame;
}

class ICHCManualOperationPageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCManualOperationPageFrame(QWidget *parent = 0);
    void AdjustFrameTransfer();
    ~ICHCManualOperationPageFrame();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);
    void timerEvent(QTimerEvent *e);
private:
    void InitInterface();
    void InitSignal();


signals:
    void ChangeDelay(int delay);

private slots:
    void StatusRefreshed();


//    void on_xPos_textChanged(const QString &arg1);

//    void on_yPos_textChanged(const QString &arg1);

//    void on_zPos_textChanged(const QString &arg1);

    void OnPointSelected(int id);
    void OnActionTriggered(int id);
    void OnShortcutTriggered(int id);
    void OnShortcutReleased(int id);

    void on_setButton_clicked();

    void on_xRun_clicked();

    void on_yRun_clicked();

#ifdef HC_AXIS_COUNT_5
    void on_zRun_clicked();
    void on_rRun_clicked();
    void on_tRun_clicked();
#endif
#ifdef HC_SK_8_SC
    void on_axisBoard_clicked();
#endif

    void on_runButton_toggled(bool checked);

    void on_productEdit_textChanged(const QString &arg1);

    void on_productClear_clicked();

    void on_singleButton_clicked();


    void on_return0Button_clicked();

    void on_serveControl_toggled(bool checked);

    void on_xSpeed_toggled(bool checked);

    void on_ySpeed_toggled(bool checked);

#ifdef HC_AXIS_COUNT_5
    void on_zSpeed_toggled(bool checked);
    void on_rSpeed_toggled(bool checked);
    void on_tSpeed_toggled(bool checked);
#endif
    void on_showMore_clicked();
    void on_returnManual_clicked();


    void on_delayEdit_textChanged(const QString &arg1);

private:
    Ui::ICHCManualOperationPageFrame *ui;
    int timerID_;

    QSignalMapper buttonSignalMapper_;
    QSignalMapper actionSignalMapper_;
    QSignalMapper shortcutSignalMapper_;
    QSignalMapper releaseShortcutSignalMapper_;
    QPushButton* nullButton_;
    AxisModifyDialog* modifyDialog_;
};

#endif // ICHCMANUALOPERATIONPAGEFRAME_H
