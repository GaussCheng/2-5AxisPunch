#ifndef ICMACHINESTRUCTPAGE_H
#define ICMACHINESTRUCTPAGE_H

#include <QWidget>
#include <QMap>

class QButtonGroup;
class QIntValidator;
class QComboBox;

namespace Ui {
    class ICMachineStructPage;
}

class ICMachineStructPage : public QWidget
{
    Q_OBJECT

public:
    explicit ICMachineStructPage(QWidget *parent = 0);
    ~ICMachineStructPage();

protected:
    void changeEvent(QEvent *e);
    void hideEvent(QHideEvent *);

private Q_SLOTS:
    void on_axisXToolButton_clicked();
    void on_axisYToolButton_clicked();
    void on_axisZToolButton_clicked();
    void on_axisPToolButton_clicked();
    void on_axisQToolButton_clicked();
    void on_axisAToolButton_clicked();
    void on_axisBToolButton_clicked();
    void on_axisCToolButton_clicked();
    void on_axisDefineToolButton_clicked();
    void on_structDefButton_clicked();
    void on_timeLimitButton_clicked();
    void on_saveToolButton_clicked();

    void SetCurrentAxis(int axis);
    void OnAxisDefineChanged(int index);
    void on_saveButton_clicked();

private:
    void InitInterface();
    QList<uint> GetCurrentStatus_() const;
    bool SetCurrentStatus_(const QList<uint> &status);
    Ui::ICMachineStructPage *ui;
    QButtonGroup* buttonGroup_;
    int currentAxis_;
    QWidget* structPage_;
    QWidget* timePage_;
    QIntValidator* maxMoveValidator_;
    QIntValidator* rotateValidator_;
    QMap<int, int> defineToIndex_;
    QMap<QComboBox*, int> boxToAxis_;
    QMap<int, int> indexToDefine_;
    int axisDefine_;
};

#endif // ICMACHINESTRUCTPAGE_H
