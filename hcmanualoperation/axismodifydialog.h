#ifndef AXISMODIFYDIALOG_H
#define AXISMODIFYDIALOG_H

#include <QDialog>
#include <QIntValidator>

namespace Ui {
class AxisModifyDialog;
}

struct ICPoint
{
    int x,y,z;
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

private:
    Ui::AxisModifyDialog *ui;
    ICPoint point_;
    QIntValidator* xValidator_;
    QIntValidator* yValidator_;
    QIntValidator* zValidator_;
};

#endif // AXISMODIFYDIALOG_H
