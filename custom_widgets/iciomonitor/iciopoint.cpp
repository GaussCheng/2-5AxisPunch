#include "iciopoint.h"

ICIOPoint::ICIOPoint(const QString &pointNum, const QString pointDescr, int hostIOPos)
{
    d = new ICIOPointData(pointNum, pointDescr, hostIOPos);
}
