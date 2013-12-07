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

private slots:
    void StatusRefreshed();


//    void on_xPos_textChanged(const QString &arg1);

//    void on_yPos_textChanged(const QString &arg1);

//    void on_zPos_textChanged(const QString &arg1);

    void OnPointSelected(int id);
    void OnActionTriggered(int id);
    void OnShortcutTriggered(int id);

    void on_setButton_clicked();

    void on_xRun_clicked();

    void on_yRun_clicked();

    void on_runButton_toggled(bool checked);

    void on_productEdit_textChanged(const QString &arg1);

    void on_productClear_clicked();

private:
    Ui::ICHCManualOperationPageFrame *ui;
    int timerID_;

    QSignalMapper buttonSignalMapper_;
    QSignalMapper actionSignalMapper_;
    QSignalMapper shortcutSignalMapper_;
    QPushButton* nullButton_;
    AxisModifyDialog* modifyDialog_;
};

#endif // ICHCMANUALOPERATIONPAGEFRAME_H
