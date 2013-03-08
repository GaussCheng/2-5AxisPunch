#ifndef ICPROGRAMGUIDEPAGE_H
#define ICPROGRAMGUIDEPAGE_H

#include "icinstructioneditorbase.h"
//#include "actionsettingframe.h"
#include <QIntValidator>
#include <QMap>
namespace Ui {
class ICProgramGuidePage;
}

class QComboBox;
class ICLineEditWithVirtualNumericKeypad;

#define AXIS_NONE 0
#define AXIS_PNEUMATIC 1
#define AXIS_SERVO 2

#define STANDBY_SETTING 1
#define GET_PRODUCT_SETTING 2
#define RELEASE_PRODUCT_SETTING 3
#define RELEASE_OUTLET_SETTING 4

#define X1_AXIS 0
#define Y1_AXIS 1
#define Z_AXIS  2
#define X2_AXIS 3
#define Y2_AXIS 4
#define A_AXIS  5
#define B_AXIS  6
#define C_AXIS  7

class ICProgramGuidePage : public ICInstructionEditorBase
{
    Q_OBJECT
    
public:
    explicit ICProgramGuidePage(QWidget *parent = 0);
    ~ICProgramGuidePage();
    
signals:
    void GuideFinished();
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;

public slots:
    void GuideKeyToActionCheck(int key);


private slots:
    void on_nextButton_clicked();

    void on_preButton_clicked();


    void on_finishButton_clicked();

    void on_usedMainArmBox_toggled(bool checked);

    void on_usedSubArmBox_toggled(bool checked);

    void on_stackedEn_toggled(bool checked);

    void on_setInButton_clicked();

private:
    struct _ICAxis_
    {
        int mode;
        int action;
        union
        {
            int standbyPos;
            int standbyLimit;
        };
        union
        {
            int getPos;
            int getLimit;
        };
        union
        {
            int releaseProductPos;
            int releaseProductLimit;
        };
        union
        {
            int releaseOutletPos;
            int releaseOutletLimit;
        };
    };

private:
    void UpdateAxisDefine_();
    void UpdatePageButton_();
    void ShowForStandby_();
    void SetAxisBoxEnabled_(bool en);
//    void ShowForProduct_();
//    void ShowForOutlet_();
    void SetAxis_(_ICAxis_* axis, int pos, int setting);
    void SetAxisPosEdit_(ICLineEditWithVirtualNumericKeypad* edit, _ICAxis_ *axis, int setting);
    void SetAxisLimitEdit_(QComboBox* edit, _ICAxis_ *axis, int setting);
    void SaveAxis_(int setting);
    bool SetAxisICMoldItem_(ICMoldItem* item, const _ICAxis_* axis, int setting) const;
    void UpdateAxisShow(int setting);
    void ShowWidgets_(QList<QWidget*> & widgets);
    void HideWidgets_(QList<QWidget*> & widgets);
    Ui::ICProgramGuidePage *ui;
    QList<QList<QWidget*> > axisWidgets_;
    QList<ICLineEditWithVirtualNumericKeypad*> posEdits_;
    QList<QComboBox*> limitEdits_;
    QMap<const _ICAxis_*, QList<int> > limitActionMap_;
    QList<int> fixtureOnAction_;
    QList<int> fixtureOffAction_;
    int axisDefine_;
    int pageIndex_;
    QIntValidator posValidators_[8];
    int posMaxs_[8];
    _ICAxis_ axis_[8];
    QIntValidator* validator ;
};

#endif // ICPROGRAMGUIDEPAGE_H
