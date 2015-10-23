#ifndef ICSIMPLESETTING_H
#define ICSIMPLESETTING_H

#include <QWidget>
#include <iclineeditwithvirtualnumerickeypad.h>
#include <QMap>
#include <QIntValidator>

namespace Ui {
class ICSimpleSetting;
}

class ICSimpleSetting : public QWidget
{
    Q_OBJECT

public:
    explicit ICSimpleSetting(QWidget *parent = 0);
    ~ICSimpleSetting();
protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    void ShowEditors();
    void InitEditors();
    void SetEditors();
    void SaveEditors();
    void InitValidators();

private slots:
    void  EditorTextChanged(QString text);

private:
    Ui::ICSimpleSetting *ui;
    QMap<ICLineEditWithVirtualNumericKeypad *,int> editToAddrs;
    QMap<int,QIntValidator *> addrToValidators;
    QList<ICLineEditWithVirtualNumericKeypad *> rotateEdits;
    QMap<ICLineEditWithVirtualNumericKeypad *,QString> rotateToAxis;

    QIntValidator *rotateValidator;
};

#endif // ICSIMPLESETTING_H
