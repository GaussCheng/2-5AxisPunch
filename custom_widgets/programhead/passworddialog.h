#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();

Q_SIGNALS:
    void LevelChanged(int);

protected:
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *e);

private slots:
    void on_buttonBox_accepted();

    void on_levelComboBox_currentIndexChanged(int index);

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
