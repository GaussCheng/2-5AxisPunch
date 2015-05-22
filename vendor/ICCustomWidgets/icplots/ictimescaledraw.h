#ifndef ICTIMESCALEDRAW_H
#define ICTIMESCALEDRAW_H

#include <QTime>
#include "qwt_scale_draw.h"
#include "ICCustomWidgets_global.h"
class ICCUSTOMWIDGETSSHARED_EXPORT ICTimeScaleDraw : public QwtScaleDraw
{
public:
    ICTimeScaleDraw( const QTime &base ):
        baseTime( base )
    {
    }
    virtual QwtText label( double v ) const
    {
        QTime upTime = baseTime.addSecs( ( int )v );
        return upTime.toString();
    }

private:
    QTime baseTime;
};

#endif // ICTIMESCALEDRAW_H
