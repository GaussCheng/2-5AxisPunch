#ifndef ICCOMMONCURVE_H
#define ICCOMMONCURVE_H

#include "qwt_plot_curve.h"
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICCommonCurve : public QwtPlotCurve
{
public:
    ICCommonCurve( const QString &title ):
        QwtPlotCurve( title )
    {
        setRenderHint( QwtPlotItem::RenderAntialiased );
    }

    void SetColor( const QColor &color )
    {
        QColor c = color;
        c.setAlpha( 150 );

        setPen( c );
//        setBrush( c );
    }

    void SetLineWidth(int width)
    {
        QPen pen = this->pen();
        pen.setWidth(width);
        setPen(pen);
    }
};

#endif // ICCOMMONCURVE_H
