#ifndef ACTIONSETTINGFRAME_H
#define ACTIONSETTINGFRAME_H

//#include <QFrame>
//#include "icframe.h"
#include "icinstructioneditorbase.h"
#include "config.h"

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

protected:
    void changeEvent(QEvent *);
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;

private:
    void InitInterface();

private:
    Ui::ActionSettingFrame *ui;
//    QString oldXPos_;
//    QString oldYPos_;
//    QString oldZPos_;
    int oXP_;
    int oYP_;
    int oZP_;
#ifdef HC_8AXIS
    int oX2P_;
    int oY2P_;
    int oAP_;
    int oBP_;
    int oCP_;
#endif
//    QList<ICLabelWrapper*> wrappers_;

private slots:
    void on_inputButton_clicked();
    void StatusRefresh();
    void on_gxButton_toggled(bool checked);
    void on_gyButton_toggled(bool checked);
    void on_gzButton_toggled(bool checked);
#ifdef HC_8AXIS
    void on_gPButton_toggled(bool checked);
    void on_gQButton_toggled(bool checked);
    void on_gAButton_toggled(bool checked);
    void on_gBButton_toggled(bool checked);
    void on_gCButton_toggled(bool checked);
#endif
};

#endif // ACTIONSETTINGFRAME_H
