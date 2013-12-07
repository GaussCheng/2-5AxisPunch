#ifndef ICPROGRAMHEADFRAME_H
#define ICPROGRAMHEADFRAME_H

#include <QFrame>

#include <QTimer>
#include "icparameterssave.h"

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
    void SetCurrentLevel(int level);

    void ChangeCurrentStatus(int status);
    void ChangeRobotOrigin(bool isOrigin);
    void ChangePunchOrigin(bool isOrigin);

Q_SIGNALS:
    void LevelChanged(int);

protected:
    void changeEvent(QEvent *e);

private:
    void InitSignal();

private slots:
    void UpdateDateTime();


private:
    Ui::ICProgramHeadFrame *ui;
    static ICProgramHeadFrame * instance_;
    QTimer pageTimer_;
    QString currentMoldName_;
    int currentStatus_;
    bool isRobotOrigin_;
    bool isPunOrigin_;
    QPixmap on, off;
    explicit ICProgramHeadFrame(QWidget *parent = 0);
};

#endif // ICPROGRAMHEADFRAME_H
