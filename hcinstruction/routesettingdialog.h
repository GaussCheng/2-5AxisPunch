#ifndef ROUTESETTINGDIALOG_H
#define ROUTESETTINGDIALOG_H

#include <QDialog>

namespace Ui {
    class RouteSettingDialog;
}

class RouteSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RouteSettingDialog(QWidget *parent = 0);
    ~RouteSettingDialog();

private:
    void InitInterface();

private:
    Ui::RouteSettingDialog *ui;

private slots:
    void on_radiusLineEdit_textChanged(QString );
    void on_cancelToolButton_clicked();
    void on_okToolButton_clicked();
};

#endif // ROUTESETTINGDIALOG_H
