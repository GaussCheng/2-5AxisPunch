#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = 0);
    int flag ;
    ~PasswordDialog();

Q_SIGNALS:
    void LevelChanged(int);

protected:
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *e);

private slots:
    void on_buttonBox_accepted();


private:
    Ui::PasswordDialog *ui;
    void InitButton();
    QButtonGroup* buttonGroup_;
private slots:
    void ShowPwdEdit();
};

#endif // PASSWORDDIALOG_H
