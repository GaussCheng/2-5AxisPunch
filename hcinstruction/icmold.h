#ifndef ICMOLD_H
#define ICMOLD_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSharedData>
#include <stdint.h>
#include <QDebug>
#include <QSharedPointer>


class ICMoldItem
{
public:
    ICMoldItem():
        seq_(0),
        num_(0),
        subNum_(-1),
        gmVal_(0),
        pos_(0),
        ifVal_(0),
        ifPos_(0),
        sVal_(0),
        dVal_(0),
        sum_(0){}

    uint Seq() const { return seq_;}    //序号
    void SetSeq(uint seq) { seq_ = seq; }
    uint Num() const { return num_;}  //步序
    void SetNum(uint nVal) { num_ = nVal; }
    uint8_t SubNum() const { return subNum_;}
    void SetSubNum(int num) { subNum_ = num;}
    uint GMVal() const { return gmVal_;}    //类别区分，0是动作组，1是夹具组
    void SetGMVal(uint gmVal) { gmVal_ = gmVal; }
    bool IsAction() const { return (!(GMVal() & 0x80));}
    bool IsClip() const { return GMVal() & 0x80;}
    bool IsEarlyEnd() const { return (IFVal() & 0x80 ) == 0x80;}
    bool IsEarlySpeedDown() const { return (IFVal() & 0x20 ) == 0x20;}

    /**
      ifVal 说明
      bit
      8   平滑处理
      7   提前结束
      6   BadProduct(点位操作)
      5   提前减速
    **/

    uint GetEarlyDownSpeed() const { return (IFVal() & 0x1F );}
    void SetEarlyEnd(bool earlyEnd) { earlyEnd ? ifVal_ |= 0x80 : ifVal_ &= 0x7F;}
    void SetEarlySpeedDown(bool earlySpeedDown) { earlySpeedDown ? ifVal_ |= 0x20 : ifVal_ &= 0xDF;}
    void SetEarlyDownSpeed(uint earlyDownSpeed) {ifVal_ = (ifVal_ & ~(0x1f)) | (earlyDownSpeed & 0x1F);}
    bool IsBadProduct() const { return (IFVal() & 0x40) == 0x40;}
    void SetBadProduct(bool badProduct) { badProduct ? ifVal_ |= 0x40 : ifVal_ &= 0xBF;}
    uint IFOtherVal() const { return IFVal() & 0x1F;}
    void SetIFOtherVal(uint val) { ifVal_ &= 0xE0; ifVal_ |= (val & 0x1F);}

    bool GetSmooth() const { return (IFVal() & 0x80) == 0x80;}
    void SetSmooth(bool smooth) { smooth ? ifVal_ |= 0x80 : ifVal_ &= 0x7F;}

    uint Action() const
    {
        if(!IsAction())
        {
            return 0;
        }
        return GMVal() & 0x7F;
    }
    void SetAction(uint action)
    {
        gmVal_ = action;
    }

    uint Clip() const
    {
        if(!IsClip())
        {
            return 0;
        }
        return GMVal() & 0x7F;
    }
    void SetClip(uint clip)
    {
        gmVal_ = clip;
        gmVal_ |= 0x80;
    }

    int Pos() const { return pos_;}    //X位置
    void SetPos(int pos) { pos_ = pos; }
    uint IFVal() const { return ifVal_;}
    void SetIFVal(uint val) { ifVal_ = val; }
    uint IFPos() const { return ifPos_;}
    void SetIFPos(uint pos) { ifPos_ = pos;}


    uint SVal() const { return sVal_;}  //速度，在clip中是次数，堆叠中是选择

    void SetSVal(uint sVal) { sVal_ = sVal; }
    uint DVal() const { return dVal_;}  //延时
    void SetDVal(uint dVal) { dVal_ = dVal; }


    uint Sum() const { return sum_;}  //
    uint ReSum() const;

    void SetValue(uint seq,
                  uint num,
                  uint8_t subNum,
                  uint gmVal,
                  uint pos,
                  uint ifVal,
                  uint ifPos,
                  uint sVal,
                  uint dVal,
                  uint sum)
    {
        seq_ = seq;
        num_ = num;
        subNum_ = subNum;
        gmVal_ = gmVal;
        pos_ = pos;
        ifVal_ = ifVal;
        ifPos_ = ifPos;
        sVal_ = sVal;
        dVal_ = dVal;
        sum_ = sum;
    }
    QByteArray ToString() const;

    int ActualPos() const
    {
        return Pos();
//        return (QString::number(Pos()) + QString::number(IFPos() & 0xF)).toInt();
    }
    void SetActualPos(int pos)
    {
//        int p = pos / 10;
//        int d = pos % 10;
        SetPos(pos);
//        ifPos_ &= 0xFFFFFFF0;
//        ifPos_ |= d;
    }

    int ActualIfPos() const
    {
        return IFPos();
//        return IFPos() >> 4;
    }

    void SetActualIfPos(uint pos)
    {
//        ifPos_ &= 0x0000000F;
//        ifPos_ |= (pos << 4);
        SetIFPos(pos);
    }

    int ActualMoldCount() const
    {
        return ((IFPos() & 0xFF) << 8) | SVal();
    }

    void SetActualMoldCount(uint count)
    {
//        SetIFVal((count >> 8) & 0xFF);
        ifPos_ &= 0xFFFFFF00;
        ifPos_ |= (count >> 8) & 0xFF;
        SetSVal(count & 0xFF);
    }

    QString Comment() const { return comment_;}
    void SetComment(const QString& comment) { comment_ = comment;}

    bool operator == (const ICMoldItem &t)const{
        return ((seq_ == t.seq_) &&
                (num_ == t.num_) &&
                (subNum_ == t.subNum_) &&
                (gmVal_ == t.gmVal_) &&
                (pos_ == t.pos_) &&
                (ifVal_ == t.ifVal_) &&
                (ifPos_ == t.ifPos_) &&
                (sVal_ == t.sVal_) &&
                (dVal_ == t.dVal_));
    }


private:
    uint seq_;
    uint num_;
    uint8_t subNum_;
    uint gmVal_;
    int pos_;
    uint ifVal_;
    uint ifPos_;
    uint sVal_;
    uint dVal_;
    QString comment_;
    mutable uint sum_;
};

class ICSubMoldUIItem
{
public:
    ICSubMoldUIItem(): isSyncItem_(false){}
    ICMoldItem* BaseItem() { return &baseItem_;}
    void SetBaseItem(const ICMoldItem& item) {baseItem_ = item;}

    bool IsSyncItem() const { return isSyncItem_;}
    void SetSyncItem(bool isSyncItem) { isSyncItem_ = isSyncItem;}

    void SetStepNum(int stepNum) {baseItem_.SetNum(stepNum);}
    void SetSubNum(int subNum) { baseItem_.SetSubNum(subNum);}
    void SubNumAdd() {baseItem_.SetSubNum(baseItem_.SubNum() + 1);}
    void SubNumMinus() {baseItem_.SetSubNum(baseItem_.SubNum() -1);}
    int SubNum() const { return baseItem_.SubNum();}

    ICMoldItem ToMoldItem() const { return baseItem_;}

    QString ToString() const; /*{ return "    " + baseItem_.ToString();}*/

private:
    ICMoldItem baseItem_;
    bool isSyncItem_;
};

class ICTopMoldUIItem
{
public:
    enum ItemType
    {
        SingleTopItem,
        ParallelToItem,
        SingleSubItem,
        SyncSubItem
    };
///////////////////////////////
    ICMoldItem* BaseItem() { return &baseItem_;}
    void SetBaseItem(const ICMoldItem& item) {baseItem_ = item;}
    void AddSubMoldUIItem(const ICSubMoldUIItem& item){subItems_.append(item);}
    void InsertSubMoldUIItem(int pos, const ICSubMoldUIItem& item) { subItems_.insert(pos, item);}
    void RemoveSubItem(int pos){ subItems_.removeAt(pos);}

    int StepNum() const { return baseItem_.Num();}
    void SetStepNum(int stepNum);
    QList<ICMoldItem> ToMoldItems() const;
    QStringList ToStringList() const;

    const ICSubMoldUIItem& at(int index) const { return subItems_.at(index);}
    ICSubMoldUIItem& at(int index) { return subItems_[index];}

    int ItemCount() const { return subItems_.size() + 1;}
    int SubItemCount() const { return subItems_.size();}
    bool IsSyncSubItem(int pos) const;
//    void ReCalSubNum();

private:
    ICMoldItem baseItem_;
    QList<ICSubMoldUIItem> subItems_;
};




class ICGroupMoldUIItem
{
public://ICTopMoldUIItem * topItem = &programList_[gIndex].at(tIndex);
    void AddToMoldUIItem(const ICTopMoldUIItem &item) { topItems_.append(item);}
    void PrependTopMoldUIItem(const ICTopMoldUIItem &item) {topItems_.prepend(item);}

    int StepNum() const { return topItems_.first().StepNum();}
    void SetStepNum(int stepNum);
    int ItemCount() const;
    int TopItemCount() const { return topItems_.size();}
    int RunableTopItemCount();
    const ICTopMoldUIItem& at(int index) const { return topItems_.at(index);}
    ICTopMoldUIItem& at(int index) { return topItems_[index];}
    ICMoldItem* MoldItemAt(int index);

    void InsertTopItem(int pos, const ICTopMoldUIItem &item) { topItems_.insert(pos, item);}
    void RemoveTopItem(int pos) { topItems_.removeAt(pos);}

    QList<ICGroupMoldUIItem> SpliteToTwoGroup(int pos) const;
    void AddOtherGroup(const ICGroupMoldUIItem& other);
    QList<ICMoldItem> ToMoldItems() const;
    QStringList ToStringList() const;
private:
    QList<ICTopMoldUIItem> topItems_;
};

class ICPointConfig{

public:
    ICPointConfig(qint32 type_,qint32 property_,qint32 delay_,qint32 speed_,qint32 smooth_,qint32 reserve_):
        type(type_),
        property(property_),
        delay(delay_),
        speed(speed_),
        smooth(smooth_),
        reserve(reserve_)   {


    }
     qint32 Type() { return type;}
     qint32 Property() { return property;}
     qint32 Delay() { return delay;}
     qint32 Speed() { return speed;}
     qint32 Smooth() { return smooth;}

     void setType(quint32 type_) { type = type_;}
     void setProperty(quint32 property_) { property = property_;}
     void setSmooth(quint32 smooth_) { smooth = smooth_;}
     void setDelay(quint32 delay_) { delay = delay_;}
     void setSpeed(quint32 speed_) { speed = speed_;}

     bool operator == (const ICPointConfig &t)const{
         return ((type == t.type) &&
                 (property == t.property) &&
                 (delay == t.delay) &&
                 (speed == t.speed) &&
                 (smooth == t.smooth) &&
                 (reserve == t.reserve));
     }
    QByteArray ToString() const;

private:
    qint32 type;
    qint32 property;
    qint32 delay;
    qint32 speed;
    qint32 smooth;
    qint32 reserve;
};


inline QByteArray ICPointConfig::ToString() const{
    QByteArray ret;

    QString tmp = (QString().sprintf("%u %u %u %u %u %u",
                                     type, property, delay,speed,smooth,reserve));
    ret = tmp.toUtf8();

    return ret;
}



inline QByteArray ICMoldItem::ToString() const
{
    QByteArray ret;

    QString tmp = (QString().sprintf("%u %u %u %u %u %u %u %u %u %u ",
                                     seq_, num_, subNum_, gmVal_, pos_, ifVal_, ifPos_, sVal_, dVal_, sum_));
    tmp += comment_;
    ret = tmp.toUtf8();
//    qDebug()<<"tmp:"<<tmp;
//    qDebug()<<"ret:"<<ret;
    return ret;
}

inline uint ICMoldItem::ReSum() const
{
    int sum = seq_ + num_ + subNum_ + gmVal_ + pos_ + ifVal_ + ifPos_ + sVal_ + dVal_;
    while(sum & 0xFF00)
    {
        sum = ((sum >> 8) & 0x00FF) + (sum & 0x00FF);
    }
    sum_ = sum;
    return sum_;
}

class ICMold : public QObject
{
    Q_OBJECT
public:
    enum ICMoldParam
    {
        point0,
        point19 = 479, //120
        Product,
        reserve,
        check1,
        check2,
        check3,
        check4,
        MoldParamCount

    };
    enum ICMoldType{
        standardType, //
        punchType   //冲床上下料类型
    };

    enum ICMoldNativeParam{
        moldType, //标准模号为0，冲床上下料为1
        pointConfig1,
        pointConfig2,
        pointConfig3,
        pointConfig4,
        ClipDelay,
        WasteUsed,
        pointProperty0,
        pointProperty19 = pointProperty0 + 19,
        MoldNativeParamCount
    };


    enum ICStatckParam
    {
        Seq,
        X_Array,
        Y_Array,
        Z_Array,
        X_Gap,
        Y_Gap,
        Z_Gap,
        CNT_X,
        CNT_Y,
        CNT_Z,

        StackParamCount
    };

    enum ACTGROUP
    {
        GNULL          =0,		//0
        GX,			//1
        GY,			//2
        GZ,			//3
        GP,			//4
        GQ,			//5
        GA,			//6
        GB,			//7
        GC,
        GTo,
        GCheckX,
        GOutY, //11
        GOneXOneY,
        GTwoXTwoY,
        GCondition, //14
        GStack,
        GARC = 22,
        GMWait = 24,
        GMOut = 25,
        GWait = 26,
        GEuOut,

//        ACTMAINUP,		//8
//        ACTMAINDOWN,	//9
//        ACTMAINFORWARD,	//10
//        ACTMAINBACKWARD,//11

//        ACTPOSEHORI,	//12   水平1
//        ACTPOSEVERT,	//13   垂直1
//        ACTVICEUP,		//14
//        ACTVICEDOWN,	//15

//        ACTVICEFORWARD,	//16
//        ACTVICEBACKWARD,//17
//        ACTGOOUT,		//18
//        ACTCOMEIN,		//19

//        ACT_PoseHori2,		//20  水平2
//        ACT_PoseVert2,   //21  垂直2

//        ACT_GASUB,
//        ACT_GAADD,
//        ACT_GBSUB,
//        ACT_GBADD,
//        ACT_GCSUB,
//        ACT_GCADD,

//        ACT_OTHER = 27,
//        ACTCHECKINPUT=28,
//        ACT_WaitMoldOpened = 29,
//        ACT_Cut,
//        ACTParallel = 31,
        ACTEND = 32,
        ACTCOMMENT
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
        ACT_CORE1_ON,
        ACT_CORE2_ON,
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
        ACT_CORE1_OFF,
        ACT_CORE2_OFF,
        ACT_AUX4,
        ACT_AUX5,
        ACT_AUX6,
        ACTCLIPEND
    };


    explicit ICMold(QObject *parent = 0);

    static ICMold* CurrentMold() { return currentMold_;}
    QString MoldName() { return moldName_;}
    static void SetCurrentMold(ICMold* mold) { currentMold_ = mold;}

    uint SyncAct() const;
    uint SyncSum() const;
    void MoldReSum() {MoldReSum(moldContent_);}
    bool ReadMoldFile(const QString& fileName, bool isLoadParams = true);
    bool ReadConfigFile(const QString& fileName);
    bool ReadPointConfigFile(const QString& fileName);


    bool ReadMoldParamsFile(const QString& fileName);

    bool SaveMoldFile(bool isSaveParams = true);
    bool SaveMoldParamsFile();
    bool SaveMoldConfigFile();
    bool SaveMoldPointFile();


    QList<ICMoldItem> MoldContent() const { return moldContent_;}
    void SetMoldContent(const QList<ICMoldItem>& moldContent) { moldContent_ = moldContent;}

    void Insert(const QList<ICMoldItem> &items) { Insert(items, moldContent_);}
    void Modify(const QList<ICMoldItem> &items) { Modify(items, moldContent_);}
    void Delete(int step) { Delete(step, moldContent_);}

    static void MoldReSum(QList<ICMoldItem>& items);
    static void Insert(const QList<ICMoldItem> &items, QList<ICMoldItem>& sourceItems);
    static void Modify(const QList<ICMoldItem> &items, QList<ICMoldItem>& sourceItems);
    static void Delete(int step, QList<ICMoldItem>& sourceItems);
    static QList<ICMoldItem> UIItemToMoldItem(const QList<ICGroupMoldUIItem>& items);
    static QList<ICGroupMoldUIItem> MoldItemToUIItem(const QList<ICMoldItem>& items);
    static QStringList UIItemsToStringList(const QList<ICGroupMoldUIItem>& items);

    QList<ICGroupMoldUIItem> ToUIItems() const { return MoldItemToUIItem(MoldContent());}

    QList<int> MoldParams() const { return moldParams_;}
    QList<QList<int> > StackParams() const { return stackParams_;}
    QList<int> StackParams(int group) const;
    QList<int> AllParams() const;

    int MoldParam(ICMoldParam param) const;
    void SetMoldParam(ICMoldParam param, int value);

    int MoldNativeParam(ICMoldNativeParam param) const;
    void SetMoldNativeParam(ICMoldNativeParam param, int value);

    QList<ICPointConfig> MoldPointConfig() const { return pointConfigs;}
    void SetMoldPointConfig(const QList<ICPointConfig>& configs) { pointConfigs = configs;}


    void UpdateSyncSum();

    int StackParam(int group, ICStatckParam param) const;
    void SetStackParam(int group, ICStatckParam param, int value);

    int LastStep() const;
signals:
    void MoldPramChanged(int, int);
    void MoldNumberParamChanged();
public slots:
private:
    QList<ICMoldItem> moldContent_;
    QList<ICPointConfig> pointConfigs;
    QList<int> moldParams_;
    QList<int> moldNativeParams_;

    QList<QList<int> > stackParams_;
    int checkSum_;
    QString moldName_;
    QString moldConfigName_;
    QString moldPointName_;

    QString moldParamName_;
    int  versoin_;
//    QList<ACTGROUP> axisActions_;
    static ICMold* currentMold_;

};

inline int ICMold::MoldParam(ICMoldParam param) const
{
//    Q_ASSERT_X(param < MoldParams().size(), "ICMold::MoldParam", "param is out of range");
    if(param >= MoldParams().size()) return 0;
    return MoldParams().at(param);
}


inline int ICMold::MoldNativeParam(ICMoldNativeParam param) const
{
//    Q_ASSERT_X(param < MoldParams().size(), "ICMold::MoldParam", "param is out of range");
    if(param >= moldNativeParams_.size()) return 0;
    return moldNativeParams_.at(param);
}


inline void ICMold::SetMoldParam(ICMoldParam param, int value)
{
//    Q_ASSERT_X(param < moldParams_.size(), "ICMold::SetMoldParams", "param is out of range");
    if(param >= moldParams_.size()) { return ;}
    moldParams_[param] = value;
//    UpdateSyncSum();
    if(param < MoldParamCount)
    {
        emit MoldPramChanged(param, value);
//        emit MoldPramChanged(MoldParamCount, checkSum_);
    }
}

inline void ICMold::SetMoldNativeParam(ICMoldNativeParam param, int value)
{
//    Q_ASSERT_X(param < moldParams_.size(), "ICMold::SetMoldParams", "param is out of range");
    if(param >= moldNativeParams_.size()) { return ;}
    moldNativeParams_[param] = value;
}


inline void ICMold::SetStackParam(int group, ICStatckParam param, int value)
{
//    Q_ASSERT_X(group < stackParams_.size(), "ICMold::SetStatckParams", "group is out of range");
//    Q_ASSERT_X(param < stackParams_.at(group).size(), "ICMold::SetStatckParams", "param is out of range");
    if(group >= stackParams_.size()) return;
    if(param >= stackParams_.at(group).size()) return;
    stackParams_[group][param] = value;
//    emit MoldPramChanged(param, value);
}


inline int ICMold::StackParam(int group, ICStatckParam param) const
{
//    Q_ASSERT_X(group < StackParams().size(), "ICMold::StatckParam", "group is out of range");
//    Q_ASSERT_X(group < StackParams().at(group).size(), "ICMold::StatckParam", "param is out of range");
    if(group >= stackParams_.size()) return 0;
    if(param >= stackParams_.at(group).size()) return 0;
    return StackParams().at(group).at(param);
}

inline QList<int> ICMold::StackParams(int group) const
{
    qDebug()<<stackParams_.size();
    if(group >= stackParams_.size()) return QList<int>();
    return StackParams().at(group);
}

inline QList<int> ICMold::AllParams() const
{
    QList<int> ret;
    ret += moldParams_;
    for(int i = 0; i != stackParams_.size(); ++i)
    {
        ret += stackParams_.at(i);
    }
    ret += checkSum_;
    return ret;
}

inline int ICMold::LastStep() const
{
    return moldContent_.last().Num();
}


#endif // ICMOLD_H
