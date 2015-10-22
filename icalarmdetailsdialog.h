#ifndef ICALARMDETAILSDIALOG_H
#define ICALARMDETAILSDIALOG_H

#include <QDialog>

namespace Ui {
class ICAlarmDetailsDialog;
}

class ICAlarmDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICAlarmDetailsDialog(QWidget *parent = 0);
    ~ICAlarmDetailsDialog();

    void ShowDetails(int alarmID);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_closeButton_clicked();

private:
    Ui::ICAlarmDetailsDialog *ui;
};

#endif // ICALARMDETAILSDIALOG_H
