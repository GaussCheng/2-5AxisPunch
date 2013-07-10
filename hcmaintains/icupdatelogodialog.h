#ifndef ICUPDATELOGODIALOG_H
#define ICUPDATELOGODIALOG_H

#include <QDialog>

namespace Ui {
class ICUpdateLogoDialog;
}

class ICUpdateLogoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ICUpdateLogoDialog(QWidget *parent = 0);
    ~ICUpdateLogoDialog();
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void on_scanPictures_clicked();

    void on_setToStartup_clicked();

    void on_setToStandby_clicked();

//    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ICUpdateLogoDialog *ui;
    QString startupPage_;
    QString standbyPage_;
};

#endif // ICUPDATELOGODIALOG_H
