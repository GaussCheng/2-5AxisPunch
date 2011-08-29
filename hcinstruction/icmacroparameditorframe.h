#ifndef ICMACROPARAMEDITORFRAME_H
#define ICMACROPARAMEDITORFRAME_H

#include <QFrame>

#include <QStackedLayout>
#include <QMap>

namespace Ui {
    class ICMacroParamEditorFrame;
}

class ICMacroParamEditorFrame : public QFrame
{
    Q_OBJECT

public:
    enum CurrentAction
    {
        None,
        Insert,
        Modify
    };
    explicit ICMacroParamEditorFrame(QWidget *parent = 0);
    ~ICMacroParamEditorFrame();

protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *);

public slots:
    void SelectCurrentStep(int currentStep);

private slots:
    void on_priorToolButton_clicked();
    void on_nextStepToolButton_clicked();
    void on_deleteToolButton_clicked();
    void on_modifyToolButton_clicked();
    void on_insertToolButton_clicked();
    void on_programSelectedComboBox_activated(int index);
    void OptionButtonClicked();
    void LoadAllRecordFileInfo();
    void GetTeachContent();

signals:
    void CurrentProgramChanged();
    void JumpToProgramEditorPage();

private:
    void InitInterface();
    void InitSignal();
    void InitParameter();

    void LoadProgram(const QString & moldName);
    void LoadMacro(int macro);

//    void LoadMoldFile(const QString & moldName);
//    void AddCommandStr(const QString & commandStr);

//    void CurrentMoldFileChanged(const QString & moldName);

private:
    Ui::ICMacroParamEditorFrame *ui;
    QWidget * actionPage_;
    QWidget * injectionPage_;
    QWidget * fixturePage_;
    QWidget * conditionPage_;

    int currentMarcoNum;
    CurrentAction currentAction_;

    QString recordPath_;

    QMap<QWidget *, QWidget *> optionButtonToPage_;
    QStackedLayout * settingStackedLayout_;
};

#endif // ICMACROPARAMEDITORFRAME_H
