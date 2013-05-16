#ifndef ICSYSTEMSTATUSFRAME_H
#define ICSYSTEMSTATUSFRAME_H

#include <QFrame>

#include "statuslabel.h"

class QString;
class ICTuneMoldDialog;

namespace Ui {
    class ICSystemStatusFrame;
}

class ICSystemStatusFrame : public QFrame
{
    Q_OBJECT

public:
    enum AutoSatus
    {
        Ready,
        Running,
        SingleCycle,
        Stopping
    };

//    static ICSystemStatusFrame* Instance()
//    {
//        if(instance_ == NULL)
//        {
//            instance_ = new ICSystemStatusFrame();
//        }
//        return instance_;
//    }
    explicit ICSystemStatusFrame(QWidget *parent = 0);
    ~ICSystemStatusFrame();

    void SetCurrentSpeed(const QString& currentSpeed);
    QString CurrentSpeed() const;

    void SetManualStatus(const StatusLabel::DisplayStatus & displayStatus);
    void SetAutoStatus(AutoSatus status);
    void SetProgramStatus(const StatusLabel::DisplayStatus & displyaStatus);
    void SetOriginStatus(const StatusLabel::DisplayStatus & displayStatus);

    void SetSystemStop();
    QSize sizeHint() const { return QSize(167, 36);}

protected:
    void changeEvent(QEvent *e);
private slots:
    void on_immButton_clicked();

private:
    Ui::ICSystemStatusFrame *ui;

//    static ICSystemStatusFrame * instance_;
    void InitInterface();
    int currentStatus_;
    QPixmap singleCycle_;
    QPixmap stopping_;
    ICTuneMoldDialog* tuneMoldDialog_;
};


#endif // ICSYSTEMSTATUSFRAME_H
