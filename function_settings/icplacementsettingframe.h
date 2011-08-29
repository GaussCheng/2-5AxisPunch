#ifndef ICPLACEMENTSETTINGFRAME_H
#define ICPLACEMENTSETTINGFRAME_H

#include <QFrame>
#include <QMap>

class QStackedLayout;

namespace Ui {
    class ICPlacementSettingFrame;
}

class ICPlacementSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICPlacementSettingFrame(QWidget *parent = 0);
    ~ICPlacementSettingFrame();

private:
    void InitInterface();
    void InitSignal();

private slots:
    void SettingButtonClicked();

private:
    Ui::ICPlacementSettingFrame *ui;

    QWidget * finishedSettingPage_;
    QWidget * defectiveProductsSettingPage_;
    QWidget * qualityControlPage_;
    QWidget * weighSettingPage_;
    QWidget * materialHandlePositionPage_;
    QWidget * otherSettingPage_;
    QMap <QWidget *, QWidget *> settingButtonToPage_;
    QStackedLayout * settingStackedLayout_;
};

#endif // ICPLACEMENTSETTINGFRAME_H
