#ifndef ICORIGINDIALOG_H
#define ICORIGINDIALOG_H

#include <QDialog>

namespace Ui {
    class ICOriginDialog;
}

class ICOriginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICOriginDialog(QWidget *parent = 0);
    ~ICOriginDialog();

Q_SIGNALS:
    void OriginStatusChanged(bool);

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void StepChanged(int step);

//    void on_buttonBox_accepted();

private:
    Ui::ICOriginDialog *ui;
};

#endif // ICORIGINDIALOG_H
