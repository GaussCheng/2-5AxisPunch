#ifndef ICHCSYSTEMSETTINGSFRAME_H
#define ICHCSYSTEMSETTINGSFRAME_H

#include <QFrame>
#include <QMap>
#ifndef Q_WS_WIN32
#include <sys/utsname.h>
#include <QButtonGroup>
#endif

class QTranslator;
class QDialog;
class QButtonGroup;

namespace Ui {
    class ICHCSystemSettingsFrame;
}

class ICHCSystemSettingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCSystemSettingsFrame(QWidget *parent = 0);
    ~ICHCSystemSettingsFrame();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private:
    void InitParameter();

private:
    void Information(bool isSuccess, const QString &msg = QString());
    bool CheckIsUsbAttached() const;
    Ui::ICHCSystemSettingsFrame *ui;
    QDialog *passwordDialog_;
    QButtonGroup* buttonGroup_;
    int currentAxis_;
    int totalL_;
    int totalH_;
    int xorSum_;
    QMap<int, QString> armXStructValueToName_;
    QMap<int, QString> armYStructValueToName_;
    QMap<int, QString> armValueToName_;

    bool testvalue;

    uint armStruct_;
#ifndef Q_WS_WIN32
    utsname osInfo_;
#endif
private slots:
    void languageBoxChange();
    void on_keyToneButton_toggled(bool checked);
    void on_saveButton_clicked();
    void on_changeButton_clicked();
    void on_extentFunctionCheckBox_toggled(bool checked);
    void on_verifyButton_clicked();
    void on_backupMachineButton_clicked();
    void on_backupSystemButton_clicked();
//    void on_backupMoldsButton_clicked();
    void on_backupAllButton_clicked();
    void on_restoreMachineButton_clicked();
    void on_restoreSystemButton_clicked();
//    void on_restoreMoldsButton_clicked();
    void on_restoreAllButton_clicked();
    void on_umountButton_clicked();
    void OnLevelChanged(int level);
    void StatusRefresh();
    void on_calibrationBtn_clicked();
    void on_brightMinus_clicked();
    void on_brightPlus_clicked();

    void on_clearButton_clicked();

    void on_limitFunctionBox_toggled(bool checked);

private:
    bool CheckRestoreSystemFiles_();
    bool CheckRestoreMachineFiles_();
//    void InitLanguageBox();
};

#endif // ICHCSYSTEMSETTINGSFRAME_H
