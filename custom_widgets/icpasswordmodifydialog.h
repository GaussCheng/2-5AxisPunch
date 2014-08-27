#ifndef ICPASSWORDMODIFYDIALOG_H
#define ICPASSWORDMODIFYDIALOG_H

#include <QDialog>

namespace Ui {
class ICPasswordModifyDialog;
}

class ICPasswordModifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICPasswordModifyDialog(QWidget *parent = 0);
    ~ICPasswordModifyDialog();

    QString OldPassword() const;
    QString NewPassword() const;

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ICPasswordModifyDialog *ui;
};

#endif // ICPASSWORDMODIFYDIALOG_H
