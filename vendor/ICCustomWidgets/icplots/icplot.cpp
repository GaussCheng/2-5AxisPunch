#include "icplot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"

ICPlot::ICPlot(QWidget *parent) :
    #ifdef Q_WS_WIN32
    QFrame(parent)
  #else
    QwtPlot(parent)
  #endif
{
    canvas()->setBorderRadius( 10 );
    plotLayout()->setAlignCanvasToScales( true );
    grid_ = new QwtPlotGrid();
    EnXGrid(true);
    EnYGrid(true);
    EnXMinGrid(false);
    EnYMinGrid(false);
//    grid_->enableX(true);    // 网格线整体开关
//    grid_->enableY(true);
//    grid_->enableXMin(false);
//    grid_->enableYMin(false);    // 最小刻度网格线
    // 网格线有两种,major和minor(最小刻度)
    // 下面是设置mojor线的样式
//    setCanvasBackground(QColor(60,60,60));
//    setMajPen(QPen(Qt::white, 0, Qt::DotLine));
//    grid_->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid_->attach(this);
//    QFont font = XAxisFont();
//    font.setPointSize(14);
//    SetXAxisFont(font);
//    SetYAxisFont(font);
}
