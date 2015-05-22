#include "ictimescaleplot.h"
#include "ictimescaledraw.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"
#include "qwt_scale_widget.h"

ICTimeScalePlot::ICTimeScalePlot(QWidget *parent) :
    ICPlot(parent),
    timeInterval_(1000),
    scaleDraw_(NULL)
{
#ifndef Q_WS_WIN32
    setAutoReplot( false );

//    canvas()->setBorderRadius( 10 );
//    plotLayout()->setAlignCanvasToScales( true );
    setAxisScale( QwtPlot::xBottom, 0, 60 );
#endif
    xScaleMin_ = 0;
    xScaleMax_ = 60;
    EnXGrid(false);
//    grid_ = new QwtPlotGrid();
//    grid_->enableX(false);    // 网格线整体开关
//    grid_->enableY(true);
//    grid_->enableXMin(false);
//    grid_->enableYMin(false);    // 最小刻度网格线
    // 网格线有两种,major和minor(最小刻度)
    // 下面是设置mojor线的样式
//    grid_->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
//    grid_->attach(this);
//    QFont font = XAxisFont();
//    font.setPointSize(14);
//    SetXAxisFont(font);
//    SetYAxisFont(font);
}

//void ICTimeScalePlot::showEvent(QShowEvent *e)
//{
//    interValTimer_ = startTimer(TimeInterval());
//    QwtPlot::showEvent(e);
//}

//void ICTimeScalePlot::hideEvent(QHideEvent *e)
//{
//    killTimer(interValTimer_);
//    QwtPlot::hideEvent(e);
//}

//void ICTimeScalePlot::timerEvent(QTimerEvent *e)
//{
//    Q_UNUSED(e);
//    ++xScaleMin_;
//    ++xScaleMax_;
//    setAxisScale( QwtPlot::xBottom,
//                  xScaleMin_, xScaleMax_ );
//    replot();
//}

void ICTimeScalePlot::SetStartupTime(const QTime &time)
{
    //    if(scaleDraw_ != NULL)
    //    {
    //        delete scaleDraw_;
    //        scaleDraw_ = NULL;
    //    }
    scaleDraw_ = new ICTimeScaleDraw(time);
#ifdef Q_WS_WIN32
#else
    setAxisScaleDraw( QwtPlot::xBottom, scaleDraw_);
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );
#endif
}

void ICTimeScalePlot::SetXAxisInterval(int interval)
{
    xScaleMax_ = xScaleMin_ + interval;
}

void ICTimeScalePlot::SetXAxisFont(const QFont &font)
{
#ifdef Q_WS_WIN32
#else
    setAxisFont(QwtPlot::xBottom, font);
    QwtScaleWidget *scaleWidget = axisWidget( QwtPlot::xBottom );
    const int fmh = QFontMetrics( scaleWidget->font() ).height();
    scaleWidget->setMinBorderDist( 0, fmh);
#endif
}
