#ifndef ICADMINPASSWORDDIALOG_H
#define ICADMINPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
    class ICAdminPasswordDialog;
}
class QAbstractButton;

class ICAdminPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICAdminPasswordDialog(QWidget *parent = 0);
    ~ICAdminPasswordDialog();

private slots:

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ICAdminPasswordDialog *ui;
};

#endif // ICADMINPASSWORDDIALOG_H
