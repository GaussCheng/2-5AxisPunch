#ifndef ICLINEEDITWRAPPER_H
#define ICLINEEDITWRAPPER_H

#include <QObject>

class QLineEdit;

class ICLineEditWrapper : public QObject
{
    Q_OBJECT
public:
    enum WrapperFor
    {
        System,
        Mold
    };
    enum DisplayFormat
    {
        Integer,
        OneFraction,
        TwoFraction
    };

    explicit ICLineEditWrapper(QLineEdit *edit, int addr, WrapperFor type, DisplayFormat format, QObject *parent = 0);
    void UpdateParam();
    //    void Binding(QLabel*)

signals:

public slots:
private Q_SLOTS:
    void EditFinished(const QString& text);

private:
    QLineEdit *wrappedWidget_;
    int addr_;
    WrapperFor type_;
    DisplayFormat format_;

};

#endif // ICLINEEDITWRAPPER_H
