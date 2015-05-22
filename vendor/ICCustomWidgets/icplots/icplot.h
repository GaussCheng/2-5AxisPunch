#ifndef ICPLOT_H
#define ICPLOT_H

#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "ICCustomWidgets_global.h"


#ifdef Q_WS_WIN32
#include <QFrame>
class ICCUSTOMWIDGETSSHARED_EXPORT ICPlot : public QFrame
        #else
class ICCUSTOMWIDGETSSHARED_EXPORT ICPlot : public QwtPlot
        #endif
{
    Q_OBJECT
public:
    explicit ICPlot(QWidget *parent = 0);

    QwtText XAxisTitle() const
    {
#ifdef Q_WS_WIN32
        return QwtText();
#else
        return axisTitle(QwtPlot::xBottom);
#endif
    }
    void SetXAxisTitle(const QString& text);
    void SetXAxisTitle(const QwtText& text)
    {
#ifdef Q_WS_WIN32
#else
        setAxisTitle(QwtPlot::xBottom, text);
#endif
    }
    void SetXAxisScale(qreal min, qreal max, qreal step)
    {
#ifdef Q_WS_WIN32
#else
        setAxisScale(QwtPlot::xBottom,
                     min,
                     max,
                     step);
        updateAxes();
#endif
    }

    QwtText YAxisTitle() const
    {
#ifdef Q_WS_WIN32
        return QwtText();
#else
        return axisTitle(QwtPlot::yLeft);
#endif
    }
    void SetYAxisTitle(const QString& text);
    void SetYAxisTitle(const QwtText& text)
    {
#ifdef Q_WS_WIN32
#else
        setAxisTitle(QwtPlot::yLeft, text);
#endif
    }
    void SetYAxisScale(qreal min, qreal max, qreal step)
    {
#ifdef Q_WS_WIN32
#else
        setAxisScale(QwtPlot::yLeft,
                     min,
                     max,
                     step);
        updateAxes();
#endif
    }

    void EnXGrid(bool enable) { grid_->enableX(enable);}
    void EnYGrid(bool enable) { grid_->enableY(enable);}
    void EnXMinGrid(bool enable) { grid_->enableXMin(enable);}
    void EnYMinGrid(bool enable) { grid_->enableYMin(enable);}
    void setMajPen(const QPen& pen) {grid_->setMajPen(pen);}

    QFont XAxisFont() const
    {
#ifdef Q_WS_WIN32
        return this->font();
#else
        return axisFont(QwtPlot::xBottom);
#endif
    }
    virtual void SetXAxisFont(const QFont& font)
    {
#ifdef Q_WS_WIN32
#else
        setAxisFont(QwtPlot::xBottom, font);
#endif
    }
    QFont YAxisFont() const
    {
#ifdef Q_WS_WIN32
        return this->font();
#else
        return axisFont(QwtPlot::yLeft);
#endif
    }
    virtual void SetYAxisFont(const QFont& font)
    {
#ifdef Q_WS_WIN32
#else
        setAxisFont(QwtPlot::yLeft, font);
#endif
    }
    
signals:
    
public slots:

private:
    QwtPlotGrid *grid_;
    
};

inline void ICPlot::SetXAxisTitle(const QString &text)
{
#ifdef Q_WS_WIN32
#else
    setAxisTitle(QwtPlot::xBottom, text);
#endif
}

inline void ICPlot::SetYAxisTitle(const QString &text)
{
#ifdef Q_WS_WIN32
#else
    setAxisTitle(QwtPlot::yLeft, text);
#endif
}

#endif // ICPLOT_H
