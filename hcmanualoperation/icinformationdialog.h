#ifndef ICINFORMATIONDIALOG_H
#define ICINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ICInformationDialog;
}

class ICInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICInformationDialog(QWidget *parent = 0);
    void setInfo(QString info);
    ~ICInformationDialog();

private:
    Ui::ICInformationDialog *ui;
};

#endif // ICINFORMATIONDIALOG_H
