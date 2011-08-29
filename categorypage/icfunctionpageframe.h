#ifndef ICFUNCTIONPAGEFRAME_H
#define ICFUNCTIONPAGEFRAME_H

#include <QFrame>
#include <QStackedLayout>
#include <QMap>

#include "iccategorypagebaseframe.h"

namespace Ui {
    class ICFunctionPageFrame;
}

class ICFunctionPageFrame : public ICCategoryPageBaseFrame
{
    Q_OBJECT

public:
    explicit ICFunctionPageFrame(QWidget *parent = 0, QStackedLayout * backFrameCenterStackedLayout = NULL);
    ~ICFunctionPageFrame();

    void ShowFunctionSelectPage();

Q_SIGNALS:
    void SettingPageShown(const QString&);
protected:
    void changeEvent(QEvent *);

private:
    void InitSettingPage();
    void InitSignal();
    void UpdateTranslate();

private slots:
    void SettingButtonClicked();

private:
    Ui::ICFunctionPageFrame *ui;

//    QWidget * signalSettingPage_;
//    QWidget * productSettingPage_;
//    QWidget * specialSettingPage_;
//    QWidget * auxiliaryMachinePage_;
    QWidget * systemSettings_;
//    QWidget * securityPointSettingPage_;
//    QWidget * standbySettingWidgetPage_;
//    QWidget * placementSettingPage_;
//    QWidget * stackSettingPage_;
//    QWidget * fixedProgramPage_;
//    QWidget * systemSettingPage_;
    friend class ICSettingFunctionFactory;
    QMap <QWidget *, QWidget *> settingButtonToPage_;
    QStackedLayout * backFrameCenterStackedLayout_;
};

#endif // ICFUNCTIONPAGEFRAME_H
