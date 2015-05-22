#ifndef ICBINDIMAGE_H
#define ICBINDIMAGE_H

#include <QLabel>
#include <QMap>
#include "ICCustomWidgets_global.h"

//class QPixmap;
class ICCUSTOMWIDGETSSHARED_EXPORT ICBindImage : public QLabel
{
    Q_OBJECT
public:
    explicit ICBindImage(QWidget *parent = 0);
    void setValue(int value);
    void bindValue(int value,QPixmap pix);

signals:

public slots:

private:
    QMap<int,QPixmap> _valueMap;
    int _currentValue;

};

#endif // ICBINDIMAGE_H
