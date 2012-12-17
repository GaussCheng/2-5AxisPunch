#ifndef ICSTRUCTDEFINEFRAME_H
#define ICSTRUCTDEFINEFRAME_H

#include <QWidget>
#include <QMap>

class QComboBox;
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
signals:
    void StructChanged();

private slots:
    void on_saveButton_clicked();
    void on_doubleArmButton_toggled(bool checked);


    void on_mainArmDownLimitButton_toggled(bool checked);

    void on_mainArmBackwardLimitButton_toggled(bool checked);

    void on_mainArmForwardLimitButton_toggled(bool checked);

    void on_subArmDownLimitButton_toggled(bool checked);

    void on_subArmBackwardLimitButton_toggled(bool checked);

    void on_subArmForwardLimitButton_toggled(bool checked);
    void on_escapeComboBox_activated(int index);

    void OnAxisDefineChanged(int index);
    void OnOutputDefineChanged(int index);

    void retranslateUi_();

private:
    Ui::ICStructDefineFrame *ui;
    uint armStruct_;
    int axisDefine_;
    int outDefine_;
    QMap<int, int> armDefineToIndex_;
    QMap<QComboBox*, int> boxToAxis_;
    QMap<int, int> indexToArmDefine_;
    QMap<QComboBox*, int> outputDefineToNumber_;
    QMap<int, QComboBox*> numberToOutputDefine_;



};

#endif // ICSTRUCTDEFINEFRAME_H
