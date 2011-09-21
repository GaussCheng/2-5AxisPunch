#ifndef AXISSETTINGSFRAME_H
#define AXISSETTINGSFRAME_H

#include <QFrame>

namespace Ui {
    class AxisSettingsFrame;
}

class AxisSettingsFrame : public QFrame
{
    Q_OBJECT

public:
    enum Axis
    {
        Hide,
        XAxis,
        YAxis,
        ZAxis
    };

    explicit AxisSettingsFrame(QWidget *parent = 0);
    ~AxisSettingsFrame();

public slots:
    void SetCurrentAxis(QString currentAxisName, int axis);

protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);

private:
    void InitInterface();
    QList<uint> GetCurrentStatus_(bool isGuild) const;
    bool SetCurrentStatus_(const QList<uint> & status);

private:
    Ui::AxisSettingsFrame *ui;
    int step_;
    bool isStart_;
    int currentAxis_;

private slots:
    void on_pushButton_clicked();
    void on_revTestPushButton_clicked();
    void on_saveToolButton_clicked();
    void on_maxPositionToolButton_clicked();
    void on_minPositionToolButton_clicked();
    void on_testPushButton_clicked();
    void StatusRefresh();
    void LevelChanged(int level);
};

#endif // AXISSETTINGSFRAME_H
