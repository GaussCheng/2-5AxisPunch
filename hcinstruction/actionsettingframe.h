#ifndef ACTIONSETTINGFRAME_H
#define ACTIONSETTINGFRAME_H

//#include <QFrame>
//#include "icframe.h"
#include "icinstructioneditorbase.h"

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
    QDialog * routeSettingPage_;
//    QString oldXPos_;
//    QString oldYPos_;
//    QString oldZPos_;
    int oXP_;
    int oYP_;
    int oZP_;
    const QString poseH;
    const QString poseV;
    const QString sAU;
    const QString sAD;
    const QString sAF;
    const QString sAB;
    int gestureAction_;
    bool isSubFarwarded_;
    bool isSubUpped_;
//    QList<ICLabelWrapper*> wrappers_;

private slots:
    void on_inputButton_clicked();
    void StatusRefresh();
    void on_gxButton_toggled(bool checked);
    void on_gyButton_toggled(bool checked);
    void on_gzButton_toggled(bool checked);
};

#endif // ACTIONSETTINGFRAME_H
