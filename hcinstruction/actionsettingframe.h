#ifndef ACTIONSETTINGFRAME_H
#define ACTIONSETTINGFRAME_H

//#include <QFrame>
//#include "icframe.h"
#include "icinstructioneditorbase.h"
#include "config.h"

#include <QIntValidator>
 #include <QMessageBox>
class ICLabelWrapper;

namespace Ui {
    class ActionSettingFrame;
}

class ActionSettingFrame : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ActionSettingFrame(QWidget *parent = 0);
    ~ActionSettingFrame();

public slots:
    void KeyToActionCheck(int key);
    void ResetServo();

protected:
    void changeEvent(QEvent *);
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;

private:
    void InitInterface();
    void UpdateAxisDefine_();
    void ShowWidgets_(QList<QWidget*> & widgets);
    void HideWidgets_(QList<QWidget*> & widgets);

private:
    Ui::ActionSettingFrame *ui;
//    QString oldXPos_;
//    QString oldYPos_;
//    QString oldZPos_;
    int oXP_;
    int oYP_;
    int oZP_;
    int oTP_;
    int oPP_;
    QIntValidator posValidators_[8];
    QIntValidator posValidator[3];
    int posMaxs_[8];
    int posMins_[8];
    int posLength_[3];
    int axisDefine_;
    QList<QList<QWidget*> > axisWidgets_;
//    QList<ICLabelWrapper*> wrappers_;

private slots:
    void on_inputButton_clicked();
    void StatusRefresh();
    void on_gxButton_toggled(bool checked);
    void on_gyButton_toggled(bool checked);
#ifdef HC_AXIS_COUNT_5
    void on_gzButton_toggled(bool checked);
    void on_gtButton_toggled(bool checked);
#endif
#ifdef HC_SK_8_SC
    void on_axisBoard_clicked();
#endif
    void on_absBox_toggled(bool checked);
    void on_gPButton_toggled(bool checked);
//    void on_arcBox_clicked(bool checked);
//    void on_slashBox_clicked(bool checked);
    void on_serveControl_toggled(bool checked);
};

#endif // ACTIONSETTINGFRAME_H
