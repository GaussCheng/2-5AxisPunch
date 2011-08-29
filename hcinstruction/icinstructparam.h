#ifndef ICINSTRUCTPARAM_H
#define ICINSTRUCTPARAM_H

#include <QMap>
#include <QStringList>

#include "icmold.h"

class ICInstructParam : public QObject
{
    Q_OBJECT
public:
    enum ACTGROUP
    {
        ACTSTOP=0,		//0
        GX,			//1
        GY,			//2
        GZ,			//3
        GXY,			//4
        GZY,			//5
        GZX,			//6
        GXZ,			//7

        ACTMAINUP,		//8
        ACTMAINDOWN,	//9
        ACTMAINFORWARD,	//10
        ACTMAINBACKWARD,//11
        ACTPOSEHORI,	//12
        ACTPOSEVERT,	//13
        ACTVICEUP,		//14
        ACTVICEDOWN,	//15

        ACTVICEFORWARD,	//16
        ACTVICEBACKWARD,//17
        ACTGOOUT,		//18
        ACTCOMEIN,		//19

        ACTMAINMIDDOWN,		//20

        ACTCHECKINPUT=28,
        ACT_WaitMoldOpened = 29,
        ACT_Cut,
        ACTParallel = 31,
        ACTEND
    };

    enum CLIPGROUP
    {
        ACTCLIP1ON,
        ACTCLIP2ON,
        ACTCLIP3ON,
        ACTCLIP4ON,
        ACTCLIP5ON,
        ACTCLIP6ON,
        ACTCLIP7ON,    //喷油
        ACTCLIP8ON,    //输送带
        ACTCLSMDON,
        ACTEJECTON,
        ACTLAYOUTON,
        ACTCLIP12ON,
        ACTCLIP13ON,
        ACTCLIP14ON,
        ACTCLIP15ON,
        ACTCLIP16ON,
        ACTCLIP1OFF,
        ACTCLIP2OFF,
        ACTCLIP3OFF,
        ACTCLIP4OFF,
        ACTCLIP5OFF,
        ACTCLIP6OFF,
        ACTCLIP7OFF,
        ACTCLIP8OFF,
        ACTCLSMDOFF,
        ACTEJECTOFF,
        ACTLAYOUTOFF,
        ACTCLIP12OFF,
        ACTCLIP13OFF,
        ACTCLIP14OFF,
        ACTCLIP15OFF,
        ACTCLIP16OFF,
        ACTCLIPEND
    };
    static ICInstructParam * Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICInstructParam;
        }
        return instance_;
    }

    static QString ConvertCommandStr(const ICMoldItem & moldItem);

    QStringList CommandStrList() { return commandStrList_; }
    void AppendCommandStrList(const QString & commandStr);

    QStringList ContentParam(const QList<ICMoldItem>& moldContent);

    QString Action(int actionID) const  { return actionGroupMap_.value(actionID, "");}
    QString Clip(int clipID) const { return clipGroupMap_.value(clipID, "");}

    QMap<int, QString> ActionGroupMap() const { return actionGroupMap_; }
    QMap<int, QString> ClipGroupMap() const { return clipGroupMap_; }

signals:
    void HostParamChanged();

public slots:
    void UpdateHostParam();
    void UpdateTranslate();

private:
    void InitSignals();
    void InstallMoldInfo();
    void InitClassesInfo();

private:
    static QMap<int, QString> actionGroupMap_;
    static QMap<int, QString> clipGroupMap_;
    static QList<int> xyzStatusList_;
    static QList<int> clipStatusList_;

    QStringList commandStrList_;
    QStringList marcoCommadnStrList_;

    static bool IsStackedAction(int action);


    static ICInstructParam * instance_;
    ICInstructParam();
};

#endif // ICINSTRUCTPARAM_H
