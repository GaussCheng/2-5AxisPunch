#ifndef ICIOMONITOR_H
#define ICIOMONITOR_H

#include <QFrame>
class ICIOMonitorPageBase;

class ICIOMonitor : public QFrame
{
    Q_OBJECT
public:
    explicit ICIOMonitor(QWidget *parent = 0);
    ~ICIOMonitor();

    void AddPage(ICIOMonitorPageBase* page, const QString &pageTitle);

    int PageCount() const;

public Q_SLOTS:

signals:

public slots:
private Q_SLOTS:
    void ShowNextPage_();
    void ShowPrePage_();
    void ShowInput_();
    void ShowOutput_();
    void SwitchPage(QWidget* widget);

private:
    class ICIOMonitorPrivate;
    QScopedPointer<ICIOMonitorPrivate> pimpl_;
};

#endif // ICIOMONITOR_H
