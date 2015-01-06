#ifndef ICSTRUCTDEFINEFRAME_H
#define ICSTRUCTDEFINEFRAME_H

#include <QWidget>
#include <QMap>
#include <QButtonGroup>

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
    QButtonGroup* buttongroup_ ;

    QList<QList<QAbstractButton*> >buttonslist_ ;
    void InitEscapeBox() ;
    QList<QPushButton*> punchButtons_;
    int timerID_;

};

#endif // ICSTRUCTDEFINEFRAME_H
