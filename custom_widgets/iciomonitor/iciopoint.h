#ifndef ICIOPOINT_H
#define ICIOPOINT_H

#include <QString>
#include <QSharedDataPointer>
#include <QSharedData>

class ICIOPointData :public QSharedData
{
public:
    ICIOPointData(){}
    ICIOPointData(const QString& pointNum, const QString pointDescr, int hostIOPos)
        :pointNum_(pointNum),
        pointDescr_(pointDescr),
        hostIOPos_(hostIOPos){}
    ~ICIOPointData(){}
    QString pointNum_;
    QString pointDescr_;
    int hostIOPos_;
};

class ICIOPoint
{
public:
    ICIOPoint(){ d = new ICIOPointData();}
    ICIOPoint(const QString& pointNum, const QString pointDescr, int hostIOPos);
    QString PointNum() const { return d->pointNum_;}
    void SetPointNum(const QString& pointNum) { d->pointNum_ = pointNum;}

    QString PointDescription() const { return d->pointDescr_;}
    void SetPointDescription(const QString &pointDescr) { d->pointDescr_ = pointDescr;}

    int HostIOPos() const { return d->hostIOPos_;}
    void SetHostIOPos(int hostIOPos) { d->hostIOPos_ = hostIOPos;}
private:
//    QString pointNum_;
//    QString pointDescr_;
//    int hostIOPos_;
    QSharedDataPointer<ICIOPointData> d;
};

#endif // ICIOPOINT_H
