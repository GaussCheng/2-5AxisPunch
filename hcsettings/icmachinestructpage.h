#ifndef ICMACHINESTRUCTPAGE_H
#define ICMACHINESTRUCTPAGE_H

#include <QWidget>
#include <QMap>
#include <QTimer>

class QButtonGroup;
class QIntValidator;
class QComboBox;
class ICStructDefineFrame;
class QPushButton;

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

//    void on_punch_clicked();

private:
    void InitInterface();
    void HideSafeZone();
    void ShowSafeZone();

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
    QIntValidator* minSecValidator_;
    QIntValidator* maxSecValidator_;
 //   QIntValidator* externalValidator_;

    int axisDefine_;
    QTimer refreshTimer_;
private slots:
    void OnConfigChanged(QObject* w, const QString& newV, const QString& oldV);
    void OnConfigChanged(const QString& text);
    void OnConfigChanged(int v);
    void OnConfigChanged(int v, int ov);
    void OnConfigChanged(bool b);
private:
    QMap<QObject*, int> editorToConfigIDs_;
};

#endif // ICMACHINESTRUCTPAGE_H
