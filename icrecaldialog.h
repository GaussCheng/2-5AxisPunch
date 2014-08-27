#ifndef ICRECALDIALOG_H
#define ICRECALDIALOG_H

#include <QDialog>

namespace Ui {
class ICRecalDialog;
}

class ICRecalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICRecalDialog(QWidget *parent = 0);
    ~ICRecalDialog();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
private:
    Ui::ICRecalDialog *ui;
};

#endif // ICRECALDIALOG_H
