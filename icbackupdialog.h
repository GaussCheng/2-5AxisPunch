#ifndef ICBACKUPDIALOG_H
#define ICBACKUPDIALOG_H

#include <QDialog>

namespace Ui {
class ICBackupDialog;
}

class ICBackupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICBackupDialog(QWidget *parent = 0);
    ~ICBackupDialog();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::ICBackupDialog *ui;
};

#endif // ICBACKUPDIALOG_H
