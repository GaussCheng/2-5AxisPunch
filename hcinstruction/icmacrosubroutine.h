#ifndef ICMACROSUBROUTINE_H
#define ICMACROSUBROUTINE_H

#include <QObject>
#include <QList>
#include <QScopedPointer>

#include "icmold.h"

//class ICMacroSubroutineItem
//{
//public:
//    uint Seq() const { return seq_;}
//    uint NVal() const { return nVal_;}
//    uint GMVal() const { return gmVal_;}
//    uint X() const { return x_;}
//    uint Y() const { return y_;}
//    uint Z() const { return z_;}
//    uint SVal() const { return sVal_;}
//    uint Sum() const { return sum_;}
//    uint RVal() const { return rVal_;}
//    uint DVal() const { return dVal_;}

//    void SetValue(uint seq,
//                  uint nVal,
//                  uint gmVal,
//                  uint x,
//                  uint y,
//                  uint z,
//                  uint sVal,
//                  uint sum,
//                  uint rVal,
//                  uint dVal)
//    {
//        seq_ = seq;
//        nVal_ = nVal;
//        gmVal_ = gmVal;
//        x_ = x;
//        y_ = y;
//        z_ = z;
//        sVal_ = sVal;
//        sum_ = sum;
//        rVal_ = rVal;
//        dVal_ = dVal;
//    }

//private:
//    uint seq_;
//    uint nVal_;
//    uint gmVal_;
//    uint x_;
//    uint y_;
//    uint z_;
//    uint sVal_;
//    uint sum_;
//    uint rVal_;
//    uint dVal_;
//};

class ICMacroSubroutine : public QObject
{
    Q_OBJECT
public:
    typedef QList<QList<ICMoldItem> > ICMacroAllSubroutine;
    static ICMacroSubroutine* Instance()
    {
        if(instance_.isNull())
        {
            instance_.reset(new ICMacroSubroutine());
        }
        return instance_.data();
    }

    bool ReadMacroSubroutieFiles(const QString& dir);
    bool SaveMacroSubroutieFile(int group);
    void SaveMacroSubroutieFiles();

    ICMacroAllSubroutine SubRoutines() const { return subroutines_;}
    QList<ICMoldItem> SubRoutine(int group) const;
    void SetSubRoutine(const QList<ICMoldItem>& items, int group);
    void Insert(const QList<ICMoldItem>& items, int group);
    void Modify(const QList<ICMoldItem>& items, int group);
    void Delete(int step, int group);
    void SubRoutineResum(int group);
    uint SyncAct() const;
    uint SyncSum() const;

    int LastStep(int group) const;

signals:

public slots:
private:
    ICMacroAllSubroutine subroutines_;
    QString subsDir_;

    static QScopedPointer<ICMacroSubroutine> instance_;
    explicit ICMacroSubroutine(QObject *parent = 0);

};

inline QList<ICMoldItem> ICMacroSubroutine::SubRoutine(int group) const
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::SubRoutine", "group is out of range");
    return subroutines_.at(group);
}

inline void ICMacroSubroutine::SetSubRoutine(const QList<ICMoldItem> &items, int group)
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::SetSubRoutine", "group is out of range");
    subroutines_[group] = items;
}

inline void ICMacroSubroutine::Insert(const QList<ICMoldItem> &items, int group)
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::Insert", "group is out of range");
    Q_ASSERT_X(!items.isEmpty(), "ICMacroSubroutine::Insert", "items is empty");
    ICMold::Insert(items, subroutines_[group]);
}

inline void ICMacroSubroutine::Modify(const QList<ICMoldItem> &items, int group)
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::Modify", "group is out of range");
    Q_ASSERT_X(!items.isEmpty(), "ICMacroSubroutine::Modify", "items is empty");
    ICMold::Modify(items, subroutines_[group]);
}

inline void ICMacroSubroutine::Delete(int step, int group)
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::Delete", "group is out of range");
    ICMold::Delete(step, subroutines_[group]);
}

inline void ICMacroSubroutine::SubRoutineResum(int group)
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::SubRoutineResum", "group is out of range");
    ICMold::MoldReSum(subroutines_[group]);
}

inline void ICMacroSubroutine::SaveMacroSubroutieFiles()
{
    for(int i = 0; i != subroutines_.size(); ++i)
    {
        SaveMacroSubroutieFile(i);
    }
}

inline int ICMacroSubroutine::LastStep(int group) const
{
    Q_ASSERT_X(group < subroutines_.size(), "ICMacroSubroutine::LastStep", "group is out of range");
    return subroutines_.at(group).last().Num();
}

#endif // ICMACROSUBROUTINE_H
