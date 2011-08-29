#ifndef ICAXISRUNNINGSTATUSFRAME_H
#define ICAXISRUNNINGSTATUSFRAME_H

//#include "icframe.h"
#include <QFrame>

//class ICAxisPositionLabelWrapper;

namespace Ui {
    class ICAxisRunningStatusFrame;
}

class ICAxisRunningStatusFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICAxisRunningStatusFrame(QWidget *parent = 0);
//    static ICAxisRunningStatusFrame * Instance()
//    {
//        if(instance_ == NULL)
//        {
//            instance_ = new ICAxisRunningStatusFrame;
//        }
//        return instance_;
//    }
    ~ICAxisRunningStatusFrame();

    void SetZAxisPosition(const QString & position);
    void SetYAxisPosition(const QString & position);
    void SetXAxisPosition(const QString & position);
//    void SetCAxisPosition(const QString & position);
//    void SetAAxisPosition(const QString & position);
//    void SetBAxisPosition(const QString & position);
//    void SetY2AxisPosition(const QString & position);
//    void SetX2AxisPosition(const QString & position);

private:
    void InitInterface();

private:
    Ui::ICAxisRunningStatusFrame *ui;
//    QList<ICAxisPositionLabelWrapper*> wrappers_;
//    static ICAxisRunningStatusFrame * instance_;
};

#endif // ICAXISRUNNINGSTATUSFRAME_H
