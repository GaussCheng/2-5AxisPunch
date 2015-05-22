#ifndef ICPASSWORDDIALOG_H
#define ICPASSWORDDIALOG_H

#include <QDialog>
#include <QEventLoop>
#include "ICCustomWidgets_global.h"

namespace Ui {
class ICPasswordDialog;
}

class ICCUSTOMWIDGETSSHARED_EXPORT ICPasswordDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ICPasswordDialog(QWidget *parent = 0);
    ~ICPasswordDialog();

    QString Passowrd() const { return passord_;}
    void SetTips(const QString& tips);
    void EnterEventLoop();
    
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);
    bool eventFilter(QObject *o, QEvent *e);
    
private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ICPasswordDialog *ui;
    QString passord_;
    QList<QList<QWidget*> > widgets_;
    QEventLoop *eventLoop_;

    int currentFocusRow_;
    int currentFocusColumn_;
};

#endif // ICPASSWORDDIALOG_H
