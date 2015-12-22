#ifndef AXISMODIFYDIALOG_H
#define AXISMODIFYDIALOG_H

#include <QDialog>
#include <QIntValidator>
#include "icvirtualhost.h"

namespace Ui {
class AxisModifyDialog;
}

struct ICPoint
{
    int x,y,z,p,q;
    int pointID;
};

class AxisModifyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AxisModifyDialog(QWidget *parent = 0);
    ~AxisModifyDialog();
    
    void StartModify(ICPoint point);
protected:
    void changeEvent(QEvent *e);

    
private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();


    void on_setButton_clicked();
    quint32 GetPointValue(quint16 pos);
    qint32 GetPosValue(ICVirtualHost::ICStatus pos);

private:
    Ui::AxisModifyDialog *ui;
    ICPoint point_;
    QIntValidator* xValidator_;
    QIntValidator* yValidator_;
    QIntValidator* zValidator_;
    QIntValidator* pValidator_;
    QIntValidator* tValidator_;
};

#endif // AXISMODIFYDIALOG_H
