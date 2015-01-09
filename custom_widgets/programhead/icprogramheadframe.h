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
    void ChangeControlStatus(bool isControled);
    void ChangStatusmoldNameLabelOperation(bool s);

public slots:
    void OnLevelChanged(int level);
    void ChangeHandSelect(int index);
    void SetHanSelectEnable(bool en);

Q_SIGNALS:
    void LevelChanged(int);
    void MoldButtonClicked();

protected:
    void changeEvent(QEvent *e);

private:
    void InitSignal();

private slots:
    void UpdateDateTime();
    void on_handSelect_currentIndexChanged(int index);


private:
    Ui::ICProgramHeadFrame *ui;
    static ICProgramHeadFrame * instance_;
    QTimer pageTimer_;
    QString currentMoldName_;
    int currentStatus_;
    bool isRobotOrigin_;
    bool isPunOrigin_;
    bool isControled_;
    QPixmap on, off;
    explicit ICProgramHeadFrame(QWidget *parent = 0);
};

#endif // ICPROGRAMHEADFRAME_H
