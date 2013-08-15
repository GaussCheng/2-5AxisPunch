#ifndef AXISSETTINGSFRAME_H
#define AXISSETTINGSFRAME_H

#include <QFrame>
#include <QTimer>

class QIntValidator;

namespace Ui {
    class AxisSettingsFrame;
}

class AxisSettingsFrame : public QFrame
{
    Q_OBJECT

public:

    explicit AxisSettingsFrame(QWidget *parent = 0);
    ~AxisSettingsFrame();

public slots:
    void SetCurrentAxis(QString currentAxisName, int axis);

protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *e);

private:
    void InitInterface();
    QList<uint> GetCurrentStatus_() const;
    bool SetCurrentStatus_(const QList<uint> & status);
    QTimer refreshTimer_;
private:
    Ui::AxisSettingsFrame *ui;
    int currentAxis_;
    QIntValidator* maxMoveValidator_;
    QIntValidator* minSecValidator_;
    QIntValidator* maxSecValidator_;

//    QIntValidator* securityMinValidator_;
//    QIntValidator* securityMinValidator_

private slots:
    void on_pushButton_clicked();
    void on_revTestPushButton_clicked();
    void on_saveToolButton_clicked();
    void on_testPushButton_clicked();
    void StatusRefresh();
    void LevelChanged(int level);
};

#endif // AXISSETTINGSFRAME_H
