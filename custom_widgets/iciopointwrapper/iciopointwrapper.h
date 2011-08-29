#ifndef ICIOPOINTWRAPPER_H
#define ICIOPOINTWRAPPER_H

#include <QObject>
#include <QPixmap>

class QLabel;

class ICIOPointWrapper : public QObject
{
    Q_OBJECT
public:
    enum Type
    {
        Input,
        Output
    };
    explicit ICIOPointWrapper(QLabel* label, int pos, Type type, QObject *parent = 0);

    void UpdateStatus();

signals:

public slots:

private:
    QLabel* wrappedWidget_;
    int pos_;
    Type type_;
    QPixmap offPixmap_;
    QPixmap inputPixmap_;
    QPixmap outputPixmap_;

};

#endif // ICIOPOINTWRAPPER_H
