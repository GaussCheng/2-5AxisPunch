#ifndef ICHCINSTRUCTIONPAGEFRAME_H
#define ICHCINSTRUCTIONPAGEFRAME_H

#include <QFrame>

#include <QMap>
#include <QStackedLayout>

#include "icmold.h"
#include "icinstructmodifydialog.h"

namespace Ui {
    class ICHCInstructionPageFrame;
}
class ActionSettingFrame;
class ICProgramGuidePage;

class ICHCInstructionPageFrame : public QFrame
{
    Q_OBJECT

public:
    enum CurrentAction
    {
        None,
        Insert,
        Modify
    };

    explicit ICHCInstructionPageFrame(QWidget *parent = 0);
    ~ICHCInstructionPageFrame();
//    void LoadFileInfoButtonClicked(const QString & moldName);

protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *);

public slots:
//    void SelectCurrentStep(int currentStep);
    void ShowServoAction(int key);

private slots:
    void on_deleteToolButton_clicked();
    void on_modifyToolButton_clicked();
    void on_insertToolButton_clicked();
//    void on_programSelectedComboBox_activated(QString );
    void OptionButtonClicked();
//    void LoadAllRecordFileInfo();

    void UpdateHostParam();

//    void NewFileCreated(const QString & moldName);
//    void DeleteFile(const QString & moldName);

    void on_commandButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void OnProgramChanged(int index, QString name);

    void on_aPlusBtn_pressed();

    void on_aMinusBtn_pressed();
    void OnActionButtonReleased();
    void OnGuideFinished();

signals:
//    void JumpToProgramEditorPage();

private:
    void InitInterface();
    void InitSignal();
    void InitParameter();
    void FindIndex_(int currentIndex, int& groupItemIndex, int& topItemIndex, int& subItemIndex);
    void UpdateUIProgramList_();

//    void LoadProgram(const QString & moldName);
//    void LoadMacro(const QString & macroName);

//    void AddCommandStr(const QString & commandStr);

//    void LoadMoldFile(const QString & moldName);
    void CurrentMoldFileChanged(const QString & moldName);

    bool SaveCurrentEdit();

private:
    Ui::ICHCInstructionPageFrame *ui;
    ActionSettingFrame * actionPage_;
    QWidget * injectionPage_;
    QWidget * fixturePage_;
    QWidget * conditionPage_;
    QWidget * flagsPage_;
    QWidget * pneumaticPage_;
    QWidget * waitConditionPage_;
    QWidget * peripheryPage_;
    QWidget * cutPage_;
    QWidget * programPage_;
    QWidget * stackPage_;
    ICProgramGuidePage * guidePage_;

    ICInstructModifyDialog *modifyDialog_;

    QString recordPath_;
    CurrentAction currentAction_;

    QMap<QWidget *, QWidget *> optionButtonToPage_;

    QList<ICGroupMoldUIItem> programList_;
    int currentEdit_;
    bool isProgramChanged_;
};

#endif // ICHCINSTRUCTIONPAGEFRAME_H
