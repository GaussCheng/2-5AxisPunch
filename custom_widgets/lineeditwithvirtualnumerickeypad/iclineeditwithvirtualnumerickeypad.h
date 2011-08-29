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

    int TransThisTextToThisInt() const;
    void SetThisIntToThisText(int inputNum);

protected:
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

private:
    VirtualNumericKeypadDialog * virtualNumericKeypadDialog_;
    int decimalPlaces_;

private slots:
    void SetCurrentText(const QString &currentText);
};

#endif // ICLINEEDITWITHVIRTUALNUMERICKEYPAD_H
