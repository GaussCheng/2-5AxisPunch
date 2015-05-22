#include "icbindimage.h"
#include <QKeyEvent>
#include <QDebug>

ICBindImage::ICBindImage(QWidget *parent) :
    QLabel(parent),
    _currentValue(-1)
{

}

void ICBindImage::setValue(int value)
{
    if(_currentValue != value )
    {
        setPixmap(_valueMap.value(value));
        _currentValue = value;
    }
}

void ICBindImage::bindValue(int value, QPixmap pix)
{
    _valueMap.insert(value,pix);
}

