#ifndef ICMACHINESTRUCTPAGE_H
#define ICMACHINESTRUCTPAGE_H

#include <QWidget>
#include <QMap>

class QButtonGroup;
class QIntValidator;

namespace Ui {
    class ICMachineStructPage;
}

class ICMachineStructPage : public QWidget
{
    Q_OBJECT

public:
    explicit ICMachineStructPage(QWidget *parent = 0);
    ~ICMachineStructPage();

    enum AxisSelect{None, AxisX, AxisY, AxisZ };

protected:
    void changeEvent(QEvent *e);
    void hideEvent(QHideEvent *);

private Q_SLOTS:
    void on_axisXToolButton_clicked();
    void on_axisYToolButton_clicked();
    void on_axisZToolButton_clicked();
    void on_structDefButton_clicked();
    void on_timeLimitButton_clicked();
    void on_saveToolButton_clicked();

    void SetCurrentAxis(AxisSelect axis);
private:
    void InitInterface();
    QList<uint> GetCurrentStatus_() const;
    bool SetCurrentStatus_(const QList<uint> &status);
    Ui::ICMachineStructPage *ui;
    QButtonGroup* buttonGroup_;
    AxisSelect currentAxis_;
    QWidget* structPage_;
    QWidget* timePage_;
    QIntValidator* maxMoveValidator_;
};

#endif // ICMACHINESTRUCTPAGE_H
