#ifndef ICHCSYSTEMSETTINGSFRAME_H
#define ICHCSYSTEMSETTINGSFRAME_H

#include <QFrame>
#include <QMap>
#include <sys/utsname.h>

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
    bool CheckIsUsbAttached() const;
    void Information(bool isSuccess);
    void SetConfig(int machineLenght,
                   int maxLength,
                   int iSafe,
                   int outsafe,
                   QString minText,
                   QString maxText,
                   QString axisName);

private:
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
    uint armStruct_;
    utsname osInfo_;
private slots:
    void on_languageComboBox_activated(int );
    void on_keyToneButton_toggled(bool checked);
    void on_saveButton_clicked();
    void on_changeButton_clicked();
    void on_extentFunctionCheckBox_toggled(bool checked);
    void on_verifyButton_clicked();
    void on_backupMachineButton_clicked();
    void on_backupSystemButton_clicked();
    void on_backupMoldsButton_clicked();
    void on_backupAllButton_clicked();
    void on_restoreMachineButton_clicked();
    void on_restoreSystemButton_clicked();
    void on_restoreMoldsButton_clicked();
    void on_restoreAllButton_clicked();
    void on_umountButton_clicked();
    void OnLevelChanged(int level);
    void on_axisZToolButton_clicked();
    void on_axisYToolButton_clicked();
    void on_axisXToolButton_clicked();
    void on_axisX2ToolButton_clicked();
    void on_axisY2ToolButton_clicked();
    void on_axisAToolButton_clicked();
    void on_axisBToolButton_clicked();
    void on_axisCToolButton_clicked();
    void on_structDefButton_clicked();
//    void on_seletHmiButton_clicked();
    void on_selectHostButton_clicked();
//    void on_saveParaButton_clicked();
    void StatusRefresh();
    void on_structSelectHostButton_clicked();
    void on_calibrationBtn_clicked();
    void on_brightnessSlider_valueChanged(int value);
};

#endif // ICHCSYSTEMSETTINGSFRAME_H
