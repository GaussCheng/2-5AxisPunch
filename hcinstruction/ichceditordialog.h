#ifndef ICHCEDITORDIALOG_H
#define ICHCEDITORDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "icmold.h"

namespace Ui {
    class ICHCEditorDialog;
}

class ICHCEditorDialog : public QDialog
{
    Q_OBJECT

public:
    static ICHCEditorDialog * Instance(QWidget * parent = 0)
    {
        if(instance_ == NULL)
        {
            instance_ = new ICHCEditorDialog(parent);
        }
        return instance_;
    }
    ~ICHCEditorDialog();

    void ResetParamInDialog(ICMoldItem moldItem);

protected:
    void changeEvent(QEvent *);

signals:
    void EnterComplete(ICMoldItem);

private:
    void InitInterface();
//    void AddActionStr(const QString & actionStr);
    ICMoldItem CommbineCurrentMoldItem();

private:
    Ui::ICHCEditorDialog *ui;
    static ICHCEditorDialog * instance_;
    ICMoldItem moldItem_;
    explicit ICHCEditorDialog(QWidget *parent = 0);

private slots:
    void on_okToolButton_clicked();
    void on_cancelToolButton_clicked();
    void on_actionsListWidget_clicked(QModelIndex index);
};

#endif // ICHCEDITORDIALOG_H
