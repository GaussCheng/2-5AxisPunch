#ifndef ICACTIONDIALOG_H
#define ICACTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class ICActionDialog;
}

class ICActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICActionDialog(QWidget *parent = 0);
    ~ICActionDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_goOnButton_clicked();

    void on_giveupButton_clicked();

private:
    Ui::ICActionDialog *ui;
};

#endif // ICACTIONDIALOG_H
