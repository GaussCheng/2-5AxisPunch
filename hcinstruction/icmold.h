#ifndef ICMOLD_H
#define ICMOLD_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSharedData>
#include <stdint.h>

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
    void SetEarlyEnd(bool earlyEnd) { earlyEnd ? ifVal_ |= 0x80 : ifVal_ &= 0x7F;}
    bool IsBadProduct() const { return (IFVal() & 0x40) == 0x40;}
    void SetBadProduct(bool badProduct) { badProduct ? ifVal_ |= 0x40 : ifVal_ &= 0xBF;}
    uint IFOtherVal() const { return IFVal() & 0x1F;}
    void SetIFOtherVal(uint val) { ifVal_ &= 0xE0; ifVal_ |= (val & 0x1F);}
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
    void SetIFPos(uint pos) { ifPos_ = pos; }
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
public:
    void AddToMoldUIItem(const ICTopMoldUIItem &item) { topItems_.append(item);}

    int StepNum() const { return topItems_.first().StepNum();}
    void SetStepNum(int stepNum);
    int ItemCount() const;
    int TopItemCount() const { return topItems_.size();}
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


inline QByteArray ICMoldItem::ToString() const
{
    QByteArray ret;
    ret = QString().sprintf("%u %u %u %u %u %u %u %u %u %u",
                            seq_, num_, subNum_, gmVal_, pos_, ifVal_, ifPos_, sVal_, dVal_, sum_).toAscii();
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
        PosMainDown,
        PosMainUp,
        PosMainOut,
        PosMainIn,
        PosViceDown,
        PosViceUp,
        PosViceIn,
        PosViceOut,
        CheckClip1,
        CheckClip2,
        CheckClip3,
        CheckClip4,
        CheckClip5,
        CheckClip6,
        CheckClip7,
        CheckClip8,
        Product,

        MoldParamCount
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
    static void SetCurrentMold(ICMold* mold) { currentMold_ = mold;}

    uint SyncAct() const;
    uint SyncSum() const;
    void MoldReSum() {MoldReSum(moldContent_);}
    bool ReadMoldFile(const QString& fileName, bool isLoadParams = true);
    bool ReadMoldParamsFile(const QString& fileName);

    bool SaveMoldFile(bool isSaveParams = true);
    bool SaveMoldParamsFile();

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
    QList<int> moldParams_;
    QList<QList<int> > stackParams_;
    int checkSum_;
    QString moldName_;
    QString moldParamName_;
//    QList<ACTGROUP> axisActions_;
    static ICMold* currentMold_;

};

inline int ICMold::MoldParam(ICMoldParam param) const
{
    Q_ASSERT_X(param < MoldParams().size(), "ICMold::MoldParam", "param is out of range");
    return MoldParams().at(param);
}

inline void ICMold::SetMoldParam(ICMoldParam param, int value)
{
    Q_ASSERT_X(param < moldParams_.size(), "ICMold::SetMoldParams", "param is out of range");
    moldParams_[param] = value;
    emit MoldPramChanged(param, value);
}

inline void ICMold::SetStackParam(int group, ICStatckParam param, int value)
{
    Q_ASSERT_X(group < stackParams_.size(), "ICMold::SetStatckParams", "group is out of range");
    Q_ASSERT_X(param < stackParams_.at(group).size(), "ICMold::SetStatckParams", "param is out of range");
    stackParams_[group][param] = value;
//    emit MoldPramChanged(param, value);
}


inline int ICMold::StackParam(int group, ICStatckParam param) const
{
    Q_ASSERT_X(group < StackParams().size(), "ICMold::StatckParam", "group is out of range");
    Q_ASSERT_X(group < StackParams().at(group).size(), "ICMold::StatckParam", "param is out of range");
    return StackParams().at(group).at(param);
}

inline QList<int> ICMold::StackParams(int group) const
{
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
