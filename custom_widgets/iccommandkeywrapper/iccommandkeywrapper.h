#ifndef ICCOMMANDKEYWRAPPER_H
#define ICCOMMANDKEYWRAPPER_H

#include <QObject>

class QAbstractButton;

class ICCommandKeyWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ICCommandKeyWrapper(QAbstractButton* button, int vKey, QObject *parent = 0);

signals:

public slots:

private slots:
    void keyPressed();

private:
    QAbstractButton* wrappedWidget_;
    int vKey_;

};

#endif // ICCOMMANDKEYWRAPPER_H
