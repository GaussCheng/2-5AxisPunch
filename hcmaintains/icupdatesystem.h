#ifndef ICUPDATESYSTEM_H
#define ICUPDATESYSTEM_H

#include <QFrame>
#include <QTimer>
#include "icprogramheadframe.h"

namespace Ui {
    class ICUpdateSystem;
}

class QSettings;

class ICUpdateSystem : public QFrame {
    Q_OBJECT
public:
//    ICUpdateSystem(QWidget *parent = 0);
    static ICUpdateSystem * Instance(QWidget *parent = 0)
    {
        if(instance_ == NULL)
        {
            instance_ = new ICUpdateSystem(parent);
        }
        return instance_;
    }
    ~ICUpdateSystem();

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

private:
    Ui::ICUpdateSystem *ui;
    static ICUpdateSystem * instance_;
    explicit ICUpdateSystem(QWidget *parent = 0);
    const QString updateIniPath_;
    const QString updateHostPath_;
    QSettings *updateSettings_;
    QSettings *updateHostSettings_;

    QTimer timer_;
//    QTimer *refresh_restTimer;
    int status_;
    QDialog* updateDialog_;
//    QMap<int, int> hostStatusToStringMap_;
    bool connectHostFlag;

    void updateHostButton();
    void writeHostButton();
    void rebootButton();
    void RestartAndUpdateTheProgram();


private slots:
    void on_updateToolButton_clicked();
    void on_refreshToolButton_clicked();
    void SystemUpdateStart();
    void RefreshUSBIniInfo();
    void QueryStatus();

    void on_connectHostButton_clicked();
    void on_updateLogoButton_clicked();
    void on_registerBtn_clicked();
    void on_generateBtn_clicked();
    void RefreshRestTime();
};

//TODEBUG
//extern ICUpdateSystem *icUpdateSystem;

#endif // ICUPDATESYSTEM_H
