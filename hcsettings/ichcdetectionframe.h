#ifndef ICHCDETECTIONFRAME_H
#define ICHCDETECTIONFRAME_H

#include <QFrame>
#include <QList>

class ICTwoSelectionComboBoxWrapper;

namespace Ui {
    class ICHCDetectionFrame;
}

class ICHCDetectionFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCDetectionFrame(QWidget *parent = 0);
    ~ICHCDetectionFrame();

protected:
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);

private slots:
    void on_detectPressureComboBox_activated(int index);

    void on_detectSecurityComboBox_activated(int index);

    void on_detectMidMoldComboBox_activated(int index);
    void OnMoldNumberParamChanged();

    void on_closeMoldStatusComboBox_activated(int index);

private:
    Ui::ICHCDetectionFrame *ui;
    QList<ICTwoSelectionComboBoxWrapper*> wrappers_;
};

#endif // ICHCDETECTIONFRAME_H
