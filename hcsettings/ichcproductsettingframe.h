#ifndef ICHCPRODUCTSETTINGFRAME_H
#define ICHCPRODUCTSETTINGFRAME_H

#include <QFrame>
#include <QList>

class ICLineEditWrapper;

namespace Ui {
    class ICHCProductSettingFrame;
}

class ICHCProductSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCProductSettingFrame(QWidget *parent = 0);
    ~ICHCProductSettingFrame();

protected:
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);

private slots:
    void on_posComboBox_activated(int index);
    void OnMoldNumberParamChanged();

private:
    Ui::ICHCProductSettingFrame *ui;
    QList<ICLineEditWrapper*> wrappers_;
};

#endif // ICHCPRODUCTSETTINGFRAME_H
