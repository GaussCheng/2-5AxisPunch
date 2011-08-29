#ifndef PAGETABSBAR_H
#define PAGETABSBAR_H

#include <QFrame>
#include <QMap>

class QAbstractButton;
class QHBoxLayout;
class QButtonGroup;

class PageTabsBar : public QFrame
{
    Q_OBJECT
public:
    PageTabsBar(QWidget *parent = 0);
    ~PageTabsBar();
    int TabCount() const { return tabsWidgetMap_.keys().size();}
    void SetTabSize(QSize size) { tabSize_ = size;}

Q_SIGNALS:
    void NeedToSwitchPage(QWidget*);

public Q_SLOTS:
    void AddTab(QWidget* tabWidget, const QString & tabName);
private Q_SLOTS:
    void TabClicked();
private:
    QMap<QAbstractButton*, QWidget*> tabsWidgetMap_;
    QMap<QWidget*, QAbstractButton*> tabsWidgetToTabButtonMap_;
    QHBoxLayout* frameLayout_;
    QButtonGroup *tabButtonGroup_;
    QSize tabSize_;


signals:

public slots:

};

#endif // PAGETABSBAR_H
