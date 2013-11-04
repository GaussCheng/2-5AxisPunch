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
        GC          =0,		//0
        GX,			//1
        GY,			//2
        GZ,			//3
        GP,			//4
        GQ,			//5
        GA,			//6
        GB,			//7

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

        ACT_PoseHori2,		//20

        ACT_PoseVert2,
        ACT_GASUB,
        ACT_GAADD,
        ACT_GBSUB,
        ACT_GBADD,
        ACT_GCSUB,
        ACT_GCADD,

        ACT_OTHER = 27,
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
        ACT_AUX1,
        ACT_AUX2,
        ACT_AUX3,
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
        ACT_AUX4,
        ACT_AUX5,
        ACT_AUX6,
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
    static QMap<int, QString> countWayMap_;

    QStringList commandStrList_;
    QStringList marcoCommadnStrList_;

    static bool IsStackedAction(int action);


    static ICInstructParam * instance_;
    ICInstructParam();
};

#endif // ICINSTRUCTPARAM_H
