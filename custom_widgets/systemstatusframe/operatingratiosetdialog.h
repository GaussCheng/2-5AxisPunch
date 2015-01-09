#ifndef OPERATINGRATIOSETDIALOG_H
#define OPERATINGRATIOSETDIALOG_H

#include <QDialog>

namespace Ui {
    class OperatingRatioSetDialog;
}

class OperatingRatioSetDialog : public QDialog
{
    Q_OBJECT
public:
    enum PulleySpeed
    {
        X1 = 1,
        X5 = 5,
        X10 = 10,
        X20 = 20,
        X50 = 50
    };

    enum GlobalSpeedStep
    {
        G_X1 = 1,
        G_X5 = 5,
        G_X10 = 10,
        G_X20 = 20
    };

    enum HandwheelAxis
    {
        Axis_X,
        Axis_Y,
        Axis_Z,
        Axis_R,
        Axis_T,
    };

    static OperatingRatioSetDialog* Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new OperatingRatioSetDialog();
        }
        return instance_;
    }

    ~OperatingRatioSetDialog();

    PulleySpeed CurrentPulleySpeed() const;
    GlobalSpeedStep CurrentGlobalSpeedStep() const;
    void SetCurrentHandwheelAxis(HandwheelAxis axis);


protected:
    void changeEvent(QEvent *e);

signals:

private:
    Ui::OperatingRatioSetDialog *ui;
    static OperatingRatioSetDialog * instance_;

    explicit OperatingRatioSetDialog(QWidget *parent = 0);

private slots:
    void on_okToolButton_clicked();
    void on_xAxisRadioButton_clicked();
    void on_yAxisRadioButton_clicked();
    void on_zAxisRadioButton_clicked();
};


#endif // OPERATINGRATIOSETDIALOG_H
