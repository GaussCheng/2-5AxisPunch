#ifndef ICFRAME_H
#define ICFRAME_H

#include <QFrame>
#include <QList>

class ICCommunicationWidget;

class ICFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ICFrame(QWidget *parent = 0);

    void RegisterToRealTimeRefresh(ICCommunicationWidget* widget)
    {
        registeredWidgets_.append(widget);
    }
    void RegisterToRealTimeRefreshUniquely(ICCommunicationWidget* widget)
    {
        if(IsWidgetRegistered(widget))
        {
            return;
        }
        registeredWidgets_.append(widget);
    }

    void UnRegisterToRealTimeRefresh(ICCommunicationWidget* widget) { registeredWidgets_.removeOne(widget);}

    int RegisteredWidgetsCount() const { return registeredWidgets_.size();}
    bool IsWidgetRegistered(ICCommunicationWidget* widget) { return registeredWidgets_.contains(widget);}

signals:

public slots:

    void UpdateRegisteredWidgets();
protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
private:
    QList<ICCommunicationWidget*> registeredWidgets_;
};


#endif // ICFRAME_H
