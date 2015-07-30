#ifndef ICModifyFrame_H
#define ICModifyFrame_H

#include <QFrame>
#include <QModelIndex>
#include <QTimer>
#include <QPair>
#include "icalarmstring.h"
#include "icconfigstring.h"

namespace Ui {
    class ICModifyFrame;
}

class ICModifyFrame : public QFrame
{
    Q_OBJECT

public:
    static ICModifyFrame * Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICModifyFrame;
        }
        return instance_;
    }
    void OnActionTriggered(int configNum, const QString& newVal, const QString& oldVal);

    ~ICModifyFrame();

protected:
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *);

private slots:
    void on_logTable_clicked(QModelIndex index);
    void OnCurrentLanguageChanged();
    void AppendNewLogInTable(const QString& dt, int  configID, const QString& newVal, const QString& oldVa);


private:
    Ui::ICModifyFrame *ui;
    ICAlarmString *alarmString_;
    static ICModifyFrame * instance_;
    const QString ModifyLogFileName;

    explicit ICModifyFrame(QWidget *parent = 0);
};

#endif // ICModifyFrame_H
