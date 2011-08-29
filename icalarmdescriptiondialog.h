#ifndef ICALARMDESCRIPTIONDIALOG_H
#define ICALARMDESCRIPTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class ICAlarmDescriptionDialog;
}

class ICAlarmDescriptionDialog : public QDialog
{
    Q_OBJECT

public:
    static ICAlarmDescriptionDialog* Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICAlarmDescriptionDialog();
        }
        return instance_;
    }
    ~ICAlarmDescriptionDialog();

    void SetAlarmDescription(const QString & alarmDescription);

private:
    Ui::ICAlarmDescriptionDialog *ui;
    static ICAlarmDescriptionDialog * instance_;
    explicit ICAlarmDescriptionDialog(QWidget *parent = 0);

private slots:
    void on_okToolButton_clicked();
};

#endif // ICALARMDESCRIPTIONDIALOG_H
