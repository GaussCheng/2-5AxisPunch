#ifndef ICNWM_H
#define ICNWM_H

#include <QScopedPointer>
#include <QHostAddress>
#include <QUuid>
#include <QTcpSocket>
#include <QObject>
#include <QMetaMethod>

#ifdef HC_NWM
#define NWM_INIT \
    refreshConfigTimerID_ = -1;
#else
#define NWM_INIT
#endif

#ifdef HC_NWM
#define NWM_HIDE() \
    if(refreshConfigTimerID_ > 0)\
    {\
        killTimer(refreshConfigTimerID_);\
    }\
    if(ICVirtualHost::GlobalVirtualHost()->IsParamChanged())\
    {\
        qDebug("Detection hide");\
        ICMold::CurrentMold()->SaveMoldParamsFile();\
        ICVirtualHost::GlobalVirtualHost()->SaveSystemConfig();\
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();\
    }\
    QMap<QWidget*, int>::iterator p = editorToMoldAddr_.begin();\
    while( p != editorToMoldAddr_.end())\
    {\
        p.key()->blockSignals(false);\
        ++p;\
    }\
    p = editorToSystemAddr_.begin();\
    while( p != editorToSystemAddr_.end())\
    {\
        p.key()->blockSignals(false);\
        ++p;\
    }\
    const QMetaObject* mo = this->metaObject();\
    int mI = mo->indexOfMethod(mo->normalizedSignature("CustomHide()"));\
    if(mI > 0)\
    {\
        mo->method(mI).invoke(this);\
    }
#else
#define NWM_HIDE
#endif

#ifdef HC_NWM
#define NWM_SHOW() \
    if(ICVirtualHost::GlobalVirtualHost()->ClientConnected())\
    {\
        refreshConfigTimerID_ = startTimer(50);\
        QMap<QWidget*, int>::iterator p = editorToMoldAddr_.begin();\
        while( p != editorToMoldAddr_.end())\
        {\
            p.key()->blockSignals(true);\
            ++p;\
        }\
        p = editorToSystemAddr_.begin();\
        while( p != editorToSystemAddr_.end())\
        {\
            p.key()->blockSignals(true);\
            ++p;\
        }\
        const QMetaObject* mo = this->metaObject();\
        int mI = mo->indexOfMethod(mo->normalizedSignature("CustomShow()"));\
        if(mI > 0)\
        {\
            mo->method(mI).invoke(this);\
        }\
    }
#else
#define NWM_SHOW()
#endif

#ifdef HC_NWM

#define NWM_FUNCTION(cl)\
void cl::timerEvent(QTimerEvent *e)\
{\
    Q_UNUSED(e);\
    RefreshConfigs();\
}\
void cl::RefreshConfigs()\
{\
    QMap<QWidget*, int>::iterator p = editorToMoldAddr_.begin();\
    ICComboBox* cob;\
    ICLineEditWithVirtualNumericKeypad* le;\
    QCheckBox*ckb;\
    ICMold* mold = ICMold::CurrentMold();\
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();\
    int v;\
    while(p != editorToMoldAddr_.end())\
    {\
        cob = qobject_cast<ICComboBox*>(p.key());\
        v = mold->MoldParam(static_cast<ICMold::ICMoldParam>(p.value()));\
        if(cob != NULL)\
        {\
            cob->setCurrentIndex(v);\
            ++p;\
            continue;\
        }\
        le = qobject_cast<ICLineEditWithVirtualNumericKeypad*>(p.key());\
        if(le != NULL)\
        {\
            le->SetThisIntToThisText(v);\
            ++p;\
            continue;\
        }\
        ckb = qobject_cast<QCheckBox*>(p.key());\
        if(ckb != NULL)\
        {\
            ckb->setChecked(v);\
            ++p;\
            continue;\
        }\
        ++p;\
        qWarning("miss mold type check");\
    }\
    p = editorToSystemAddr_.begin();\
    while(p != editorToSystemAddr_.end())\
    {\
        cob = qobject_cast<ICComboBox*>(p.key());\
        v = host->SystemParameter(static_cast<ICVirtualHost::ICSystemParameter>(p.value())).toInt();\
        if(cob != NULL)\
        {\
            cob->setCurrentIndex(v);\
            ++p;\
            continue;\
        }\
        le = qobject_cast<ICLineEditWithVirtualNumericKeypad*>(p.key());\
        if(le != NULL)\
        {\
            le->SetThisIntToThisText(v);\
            ++p;\
            continue;\
        }\
        ckb = qobject_cast<QCheckBox*>(p.key());\
        if(ckb != NULL)\
        {\
            ckb->setChecked(v);\
            ++p;\
            continue;\
        }\
        ++p;\
        qWarning("miss system type check");\
    }\
    const QMetaObject* mo = this->metaObject();\
    int mI = mo->indexOfMethod(mo->normalizedSignature("CustomRefreshConfigs()"));\
    if(mI > 0)\
    {\
        mo->method(mI).invoke(this);\
    }\
}
#else
#define NWM_FUNCTION(cl)
#endif


class ICNWM;
typedef ICNWM* ICNWMPTR;
typedef QUuid ICUuid;

//struct ICServerAddress
//{
//    QHostAddress hostAddr;
//    quint16 port;
//};

class ICNWM: public QObject
{
    Q_OBJECT
public:
    static ICNWMPTR Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = (new ICNWM());
        }
        return instance_;
    }

    void DevConnect(ICUuid uuid, const QString& ip, quint16 port);
    bool IsConnected() const { return isConnected_; }

    void SendStatusToHost(const QMap<int , int> status);
    void SendStatusToHost(int status, int value);
    void PostMoldAct(const QString& name, const QString& act);
    void PostMoldFnc(const QString& name, const QString& fnc);
    void PostMoldSubs(const QString& name, const QStringList& subs);
    void PostAxisConfig(const QString& name, const QString& configs);
    void PostAxisDR(const QString& name, const QString& configs);
    void PostSystemConfig(const QString& name, const QString& configs);
    void PostPanelConfig(const QString& name, const QString& configs);

signals:
    void DevConnected();
private slots:
    void OnConnectedHost();

private:
    ICNWM();
    static ICNWMPTR instance_;
    QTcpSocket socket_;
    ICUuid uuid_;
    bool isConnected_;
//    QString ip_;
//    QString port_;
//    ICServerAddress serverAddr_;

};

#endif // ICNWM_H
