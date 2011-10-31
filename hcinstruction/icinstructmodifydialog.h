#ifndef ICINSTRUCTMODIFYDIALOG_H
#define ICINSTRUCTMODIFYDIALOG_H

#include <QDialog>

class ICMoldItem;

namespace Ui {
    class ICInstructModifyDialog;
}

class ICInstructModifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICInstructModifyDialog(QWidget *parent = 0);
    ~ICInstructModifyDialog();

    bool ShowModifyItem(ICMoldItem* item);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_earlyEndCheckBox_toggled(bool checked);

    void on_setButton_clicked();

private:
    Ui::ICInstructModifyDialog *ui;
    ICMoldItem* currentItem;
};

#endif // ICINSTRUCTMODIFYDIALOG_H
