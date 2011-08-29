#ifndef ICIOMONITORPAGEBASE_H
#define ICIOMONITORPAGEBASE_H

#include <QStackedWidget>
#include "iciopoint.h"

class ICIOMonitorPageBase : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ICIOMonitorPageBase(QWidget *parent = 0);
    ~ICIOMonitorPageBase();

    void BindingPointsToInputPage(const QList<ICIOPoint> &points);
    void BindingPointsToOutpuPage(const QList<ICIOPoint> &points);

    int PagePointsCount() const { return pagePointsCount_;}
    void SetPagePointsCount(int count) { pagePointsCount_ = count;}

signals:

public slots:
    void ShowInputPage();
    void ShowOutPage();
    void ShowElectricAdjustmentBitPage();
    void ShowNextPage();
    void ShowPrePage();

protected:
    QWidget* inputPage_;
    QWidget* outputPage_;
    QWidget* electricAdjustmentBitPage_;

private:
    void BindingPointToPage_(const QList<ICIOPoint> &points, QStackedWidget* page, int type);

    int pagePointsCount_;

};

#endif // ICIOMONITORPAGEBASE_H
