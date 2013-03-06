#ifndef ICPROGRAMHEADFRAME_H
#define ICPROGRAMHEADFRAME_H

#include <QFrame>

#include <QTimer>

namespace Ui {
    class ICProgramHeadFrame;
}

class ICProgramHeadFrame : public QFrame
{
    Q_OBJECT

public:

    static ICProgramHeadFrame * Instance(QWidget *parent = 0)
    {
        if(instance_ == NULL)
        {
            instance_ = new ICProgramHeadFrame(parent);
        }
        return instance_;
    }
    ~ICProgramHeadFrame();

    void SetCurrentMoldName(const QString & moldName);
    QString CurrentMoldName() { return currentMoldName_; }

//    void SetCurrentCategoryName(const QString & categoryName);
    int CurrentLevel() const;
    QTimer restTime_;
    void SetCurrentLevel(int level);

    void StartAutoTime(){if(!autoTime_.isActive()) autoTime_.start(60000);}
    void StopAutoTime(){autoMin_ = 0; autoTime_.stop(); UpdateAutoTime();}

Q_SIGNALS:
    void LevelChanged(int);

protected:
    void changeEvent(QEvent *e);

private:
    void InitSignal();

private slots:
    void UpdateDateTime();
    void UpdateAutoTime();

public slots:
    void ReashRestTime();


private:
    Ui::ICProgramHeadFrame *ui;
    static ICProgramHeadFrame * instance_;
    QTimer pageTimer_;
    QTimer autoTime_;
//    QTimer restTime_;
    int autoMin_;
    QString currentMoldName_;
    explicit ICProgramHeadFrame(QWidget *parent = 0);
};

#endif // ICPROGRAMHEADFRAME_H
