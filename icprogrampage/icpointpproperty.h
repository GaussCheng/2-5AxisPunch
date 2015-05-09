#ifndef ICPOINTPPROPERTY_H
#define ICPOINTPPROPERTY_H

#include <QDialog>
#include <QCheckBox>
#include <QList>
#include <QMap>

namespace Ui {
class ICPointpProperty;
}


#define USE_PROPERTY 3


enum PointProperty{
    PointProperty1,
    PointProperty2,
    PointProperty3,
    PointProperty4,
    PointProperty5,
    PointProperty6,
    PointProperty7,
    PointProperty8,
    PointProperty9,

    NULL_Property = -1

} ;

class ICPointpProperty : public QDialog
{
    Q_OBJECT
    
public:
    explicit ICPointpProperty(QWidget *parent = 0);
    ~ICPointpProperty();
    quint32 GetPointProperty();
    void SetPointProperty(quint32 p);

private:
    Ui::ICPointpProperty *ui;
    QList<QCheckBox*> boxs;
    QMap<QCheckBox*,PointProperty> boxToproperty;
};

#endif // ICPOINTPPROPERTY_H
