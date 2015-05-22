#ifndef ICTIMESCALEPLOT_H
#define ICTIMESCALEPLOT_H

#include "ICCustomWidgets_global.h"
#include "icplot.h"

class ICTimeScaleDraw;

class ICCUSTOMWIDGETSSHARED_EXPORT ICTimeScalePlot : public ICPlot
{
    Q_OBJECT
public:
    explicit ICTimeScalePlot(QWidget *parent = 0);
    int TimeInterval() const { return timeInterval_;}
    //    QString XAxisTitle() const { return axisTitle(QwtPlot::xBottom).text();}
//    QwtText XAxisTitle() const { return axisTitle(QwtPlot::xBottom);}
//    void SetXAxisTitle(const QString& text);
//    void SetXAxisTitle(const QwtText& text) { setAxisTitle(QwtPlot::xBottom, text);}
    void SetXAxisInterval(int interval);

//    QwtText YAxisTitle() const { return axisTitle(QwtPlot::yLeft);}
//    void SetYAxisTitle(const QString& text);
//    void SetYAxisTitle(const QwtText& text) { setAxisTitle(QwtPlot::yLeft, text);}

    void SetStartupTime(const QTime& time);

//    void EnGrid(bool enable) { grid_->enableY(enable);}
//    void setMajPen(const QPen& pen) {grid_->setMajPen(pen);}

//    QFont XAxisFont() const { return axisFont(QwtPlot::xBottom);}
    virtual void SetXAxisFont(const QFont& font);
//    QFont YAxisFont() const { return axisFont(QwtPlot::yLeft);}
//    void SetYAxisFont(const QFont& font) { setAxisFont(QwtPlot::yLeft, font);}

    void SetXAxisScaleRange(int min, int max)
    {
#ifdef Q_WS_WIN32
#else
        setAxisScale(QwtPlot::xBottom, min, max); replot();
#endif
    }

signals:

public slots:

protected:
    //    void showEvent(QShowEvent *e);
    //    void hideEvent(QHideEvent *e);
    //    void timerEvent(QTimerEvent *e);

private:
    int timeInterval_;          //<X轴时间间隔单位ms
    //    int interValTimer_;
    int xScaleMin_;
    int xScaleMax_;
    ICTimeScaleDraw* scaleDraw_;
//    QwtPlotGrid *grid_;

};

//inline void ICTimeScalePlot::SetXAxisTitle(const QString &text)
//{
//    setAxisTitle(QwtPlot::xBottom, text);
//}

//inline void ICTimeScalePlot::SetYAxisTitle(const QString &text)
//{
//    setAxisTitle(QwtPlot::yLeft, text);
//}

#endif // ICTIMESCALEPLOT_H
