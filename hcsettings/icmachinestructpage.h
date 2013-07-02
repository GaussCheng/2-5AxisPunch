#ifndef ICMACHINESTRUCTPAGE_H
#define ICMACHINESTRUCTPAGE_H

#include <QWidget>
#include <QMap>
#include <QTimer>

class QButtonGroup;
class QIntValidator;
class QComboBox;
class ICStructDefineFrame;

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
    void showEvent(QShowEvent *e);

private Q_SLOTS:
    void on_axisXToolButton_clicked();
    void on_axisYToolButton_clicked();
    void on_axisZToolButton_clicked();
    void on_axisPToolButton_clicked();
    void on_axisQToolButton_clicked();
    void on_axisAToolButton_clicked();
    void on_axisBToolButton_clicked();
    void on_axisCToolButton_clicked();
//    void on_axisDefineToolButton_clicked();
    void on_structDefButton_clicked();
    void on_timeLimitButton_clicked();
    void on_saveToolButton_clicked();

    void SetCurrentAxis(int axis);
    void UpdateAxisDefine_();

    void on_testPushButton_clicked();

    void on_revTestPushButton_clicked();

    void on_pushButton_clicked();

    void StatusRefresh();

    void on_mechanicalLengthLineEdit_textChanged(const QString &arg1);

    void on_maximumDisplacementLineEdit_textChanged(const QString &arg1);

private:
    void InitInterface();
    QList<uint> GetCurrentStatus_() const;
    bool SetCurrentStatus_(const QList<uint> &status);
    Ui::ICMachineStructPage *ui;
    QButtonGroup* buttonGroup_;
    int currentAxis_;
    ICStructDefineFrame* structPage_;
    QWidget* timePage_;
    QIntValidator* maxMoveValidator_;
    QIntValidator* rotateValidator_;
    QIntValidator* maximumValidator_;
    QIntValidator* intValidator;
    QIntValidator* originValidator_;
 //   QIntValidator* externalValidator_;

    int axisDefine_;
    QTimer refreshTimer_;
};

#endif // ICMACHINESTRUCTPAGE_H
