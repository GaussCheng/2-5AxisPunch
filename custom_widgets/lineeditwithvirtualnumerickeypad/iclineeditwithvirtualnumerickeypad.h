#ifndef ICLINEEDITWITHVIRTUALNUMERICKEYPAD_H
#define ICLINEEDITWITHVIRTUALNUMERICKEYPAD_H

#include <QLineEdit>

class VirtualNumericKeypadDialog;

class ICLineEditWithVirtualNumericKeypad : public QLineEdit
{
    Q_OBJECT
public:
    ICLineEditWithVirtualNumericKeypad(QWidget * parent = 0);
    ~ICLineEditWithVirtualNumericKeypad();

    int DecimalPlaces() const { return decimalPlaces_; }
    void SetDecimalPlaces(int decimalPlaces) { decimalPlaces_ = decimalPlaces; }
    bool IsModalKeyboard() const { return isModalKeyboard_;}
    void SetModalKeyboard(bool isModalkeyboard) { isModalKeyboard_ = isModalkeyboard;}

    int TransThisTextToThisInt() const;
    void SetThisIntToThisText(int inputNum);
    bool SetCurrentText(const QString &currentText);

    QString LastValue() const { return oldVal_;}

protected:
//    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void wheelEvent(QWheelEvent *);

private:
    VirtualNumericKeypadDialog * virtualNumericKeypadDialog_;
    int decimalPlaces_;
    bool isModalKeyboard_;
    QString oldVal_;

public slots:
};

class ICIncrementalLineEdit: public ICLineEditWithVirtualNumericKeypad
{
    Q_OBJECT
public:
    ICIncrementalLineEdit(QWidget * parent = 0);
public slots:
    void SetCurrentText(const QString &currentText);
};

#endif // ICLINEEDITWITHVIRTUALNUMERICKEYPAD_H
