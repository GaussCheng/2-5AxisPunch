#ifndef ICHCSYSTEMSETTINGSFRAME_H
#define ICHCSYSTEMSETTINGSFRAME_H

#include <QFrame>
#include <QMap>
#include <QProcess>
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
    void UpdateConfigShow_();
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

    void on_generateBtn_clicked();
    void on_registerBtn_clicked();

    void on_verifySupperButton_clicked();

    void on_factoryCode_textChanged(const QString &arg1);

    void on_nwmSave_clicked();

    void on_connectHost_clicked();

    void RefreshIPInfo();
    void OnScanAPFinished();
    void OnObtainIP();
    void on_sanAPBtn_clicked();
    void on_connectWifiBtn_clicked();

    void on_saveNetwork_clicked();

private:
    bool CheckRestoreSystemFiles_();
    bool CheckRestoreMachineFiles_();

private slots:
    void OnConfigChanged(QObject* w, const QString& newV, const QString& oldV);
    void OnConfigChanged(const QString& text);
    void OnConfigChanged(int v);
    void OnConfigChanged(int v, int ov);
    void OnConfigChanged(bool b);
private:
    QProcess scanAPProcess;
    QProcess connectAPProcess;
    QMap<QObject*, int> editorToConfigIDs_;
};

#endif // ICHCSYSTEMSETTINGSFRAME_H
