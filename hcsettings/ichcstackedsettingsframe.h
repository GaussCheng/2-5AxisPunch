#ifndef ICHCSTACKEDSETTINGSFRAME_H
#define ICHCSTACKEDSETTINGSFRAME_H

#include <QFrame>

namespace Ui {
    class ICHCStackedSettingsFrame;
}

class ICHCStackedSettingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCStackedSettingsFrame(QWidget *parent = 0);
    ~ICHCStackedSettingsFrame();

protected:
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);

private:
    Ui::ICHCStackedSettingsFrame *ui;

private slots:
    void on_page0ToolButton_clicked();
    void on_page1ToolButton_clicked();
    void on_page2ToolButton_clicked();
    void on_page3ToolButton_clicked();
    void OnMoldNumberParamChanged();
    void TimeOutToReconfig();

private:
    void InitInterface();
    QList<int> GetCurrentStatus_() const;
    void SetStackStatus_(const QList<int> & status);
    void RefreshStackParams_(int group);
    int currentPage_;
};

#endif // ICHCSTACKEDSETTINGSFRAME_H
