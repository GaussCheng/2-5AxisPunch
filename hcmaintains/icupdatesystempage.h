#ifndef ICUpdateSystemPage_H
#define ICUpdateSystemPage_H

#include <QFrame>
#include <QTimer>
#include "icupdatepackmodel.h"
#include "icprogramheadframe.h"
#include "icupdatesystem.h"

namespace Ui {
    class ICUpdateSystemPage;
}

class QSettings;

class ICUpdateSystemPage : public QFrame {
    Q_OBJECT
public:
//    ICUpdateSystemPage(QWidget *parent = 0);
    static ICUpdateSystemPage * Instance(QWidget *parent = 0)
    {
        if(instance_ == NULL)
        {
            instance_ = new ICUpdateSystemPage(parent);
        }
        return instance_;
    }
    ~ICUpdateSystemPage();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
signals:
    void RegisterSucceed();
private:
    void InitInterface();
    int Register(const QString& , const QString& );

private slots:
    void on_generateBtn_clicked();
    void on_registerBtn_clicked();
    void RefreshRestTime();


private:
    Ui::ICUpdateSystemPage *ui;
    static ICUpdateSystemPage * instance_;
    explicit ICUpdateSystemPage(QWidget *parent = 0);
    const QString updateIniPath_;
    QString updateHostPath_;
    QSettings *updateSettings_;
    QSettings *updateHostSettings_;
    QString updateHostFile_;

    QTimer timer_;
//    QTimer *refresh_restTimer;
    int status_;
    QDialog* updateDialog_;
//    QMap<int, int> hostStatusToStringMap_;
    bool connectHostFlag;

    QString fileName;
    ICUpdatePackModel* model_;
    ICUpdateSystem* updateSystem_;
    QTimer *refresh_restTimer;

    void updateHostButton();
    void writeHostButton();
    void rebootButton();
    void RestartAndUpdateTheProgram();
    bool CheckIsUsbAttached() const ;


private slots:
    void on_updateToolButton_clicked();
    void SystemUpdateStart();
    void RefreshUSBIniInfo();
    void QueryStatus();

    void on_connectHostButton_clicked();
    void on_updateLogoButton_clicked();
//    void on_registerBtn_clicked();
//    void on_generateBtn_clicked();
//    void RefreshRestTime();
    void on_updatePasswardButton_clicked();
    void on_scanPanel_clicked();
    void on_scanHost_clicked();
};

//TODEBUG
//extern ICUpdateSystemPage *ICUpdateSystemPage;

#endif // ICUpdateSystemPage_H
