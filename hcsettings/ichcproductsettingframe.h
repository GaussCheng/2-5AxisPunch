#ifndef ICHCPRODUCTSETTINGFRAME_H
#define ICHCPRODUCTSETTINGFRAME_H

#include <QFrame>
#include <QList>
#include <QButtonGroup>
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
    void OnMoldNumberParamChanged();

    void on_productClearButton_clicked();

    void FixtureBoxChange();
    void retranslateUi_();

    void on_countUnitBox_currentIndexChanged(int index);

    void on_getFailWay_activated(int index);

private:
    Ui::ICHCProductSettingFrame *ui;
    QList<ICLineEditWrapper*> wrappers_;
    QButtonGroup* buttongroup_ ;
    void InitCheckBox();
};

#endif // ICHCPRODUCTSETTINGFRAME_H
