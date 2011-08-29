#ifndef ICUPDATESYSTEM_H
#define ICUPDATESYSTEM_H

#include <QFrame>

namespace Ui {
    class ICUpdateSystem;
}

class QSettings;

class ICUpdateSystem : public QFrame {
    Q_OBJECT
public:
    ICUpdateSystem(QWidget *parent = 0);
    ~ICUpdateSystem();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    void InitInterface();

private:
    Ui::ICUpdateSystem *ui;

    const QString updateIniPath_;
    QSettings *updateSettings_;

    void RestartAndUpdateTheProgram();

private slots:
    void on_updateToolButton_clicked();
    void on_refreshToolButton_clicked();
    void SystemUpdateStart();
    void RefreshUSBIniInfo();
};

//TODEBUG
extern ICUpdateSystem *icUpdateSystem;

#endif // ICUPDATESYSTEM_H
