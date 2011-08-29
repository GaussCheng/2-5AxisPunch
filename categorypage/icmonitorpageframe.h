#ifndef ICMONITORPAGEFRAME_H
#define ICMONITORPAGEFRAME_H

#include <QFrame>

namespace Ui {
    class ICMonitorPageFrame;
}

class ICIOMonitorPageBase;

class ICMonitorPageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICMonitorPageFrame(QWidget *parent = 0);
    ~ICMonitorPageFrame();

protected:
    void changeEvent(QEvent *e);

private:
    void Init_();

private:
    Ui::ICMonitorPageFrame *ui;
    ICIOMonitorPageBase *otherPageLeft_;
    ICIOMonitorPageBase *otherPageRight_;
    ICIOMonitorPageBase *euPageLeft_;
    ICIOMonitorPageBase *euPageRight_;
};

#endif // ICMONITORPAGEFRAME_H
