#ifndef ICSTRUCTDEFINEFRAME_H
#define ICSTRUCTDEFINEFRAME_H

#include <QWidget>
#include <QMap>
#include <QButtonGroup>
#include <QCheckBox>


class QComboBox;
class QPushButton;
namespace Ui {
class ICStructDefineFrame;
}

class ICStructDefineFrame : public QWidget
{
    Q_OBJECT

public:
    explicit ICStructDefineFrame(QWidget *parent = 0);
    ~ICStructDefineFrame();


protected:
    void changeEvent(QEvent *e);
    void hideEvent(QHideEvent *);
    void timerEvent(QTimerEvent *);
signals:
    void StructChanged();

private slots:
    void on_saveButton_clicked();

    void escapeBoxChange();

    void OnAxisDefineChanged(int index);

    void retranslateUi_();

    //    void on_adjUse_toggled(bool checked);

    void on_tabWidget_currentChanged(int index);

    void on_oStartBtn_clicked();

    void on_oX1Btn_clicked();

    void on_oY1Btn_clicked();

    void on_oZBtn_clicked();

    void on_oX2Btn_clicked();

    void on_oY2Btn_clicked();

    void on_setOrigin_clicked();
private:
    Ui::ICStructDefineFrame *ui;
    int axisDefine_;
    QMap<int, int> armDefineToIndex_;
    QMap<QComboBox*, int> boxToAxis_;
    QMap<int, int> indexToArmDefine_;
    QMap<QButtonGroup*, int> outputDefineToNumber_;
    QList<QButtonGroup*>buttongrouplist ;
    QList<QCheckBox*> inputBoxs,ouputBoxs;
    QButtonGroup* buttongroup_ ;

    QList<QList<QAbstractButton*> >buttonslist_ ;
    void InitEscapeBox() ;
    void InitCombobox();
    void InitEnfoce();
    void InitEnforceInput();
    void InitEnforceOutput();
    quint32 enforceInputs();
    quint32 enforceOutputs();
    QList<QPushButton*> punchButtons_;
    int timerID_;
private slots:
    void OnConfigChanged(QObject* w, const QString& newV, const QString& oldV);
    void OnConfigChanged(const QString& text);
    void OnConfigChanged(int v);
    void OnConfigChanged(int v, int ov);
    void OnConfigChanged(bool b);
private:
    QMap<QObject*, int> editorToConfigIDs_;
};

#endif // ICSTRUCTDEFINEFRAME_H
