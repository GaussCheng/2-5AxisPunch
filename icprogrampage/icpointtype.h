#ifndef ICPOINTTYPE_H
#define ICPOINTTYPE_H

#include <QDialog>
#include <QCheckBox>
#include <QMap>
#include "icmold.h"

namespace Ui {
class ICPointType;
}


enum PointProperty{
    OUYY37_ON,  //吸1阀
    OUYY37_OFF,
    OUYY40_ON,  //吸2阀
    OUYY40_OFF,
    OUYY22_ON,  //夹1阀
    OUYY22_OFF,
    OUYY23_ON,  //预留阀
    OUYY23_OFF,
    WAIT_X41,
    WAIT_X42,
    WAIT_X43,
    WAIT_X44,
    RESEARVE,
    SMOOTH,
    WAITSAFE,

    NULL_Property = -1

} ;


enum PointType{
    Get_Wait,   //0
    Get_Up,     //1
    Get,        //2
    Get_Wait2,  //3
    Put_Wait,   //4
    Put_Up,     //5
    Put,        //6
    Put_Wait2,  //7
    Point_Property, //8
    Reserve,   //9
    Get_Wait3,   //10
    Wait_Safe,  //11

    NULL_Type = -1

} ;

class ICPointType : public QDialog
{
    Q_OBJECT

public:
    static ICPointType* Instance(QWidget *parent = 0)
    {
        if(!instance_)
        {
            instance_ = new ICPointType(parent);
        }
        return instance_;
    }
    ~ICPointType();
    void Init_();

    QString toString(PointType type = NULL_Type);
    QString toString(PointProperty type = NULL_Property);
    QString toPropertyString(PointProperty type = NULL_Property,quint32 delay = 0);


    ICPointConfig config() ;

    PointProperty currentPropertyType();


protected:
    void changeEvent(QEvent *);
private:
    explicit ICPointType(QWidget *parent = 0);
private slots:
    void stateChanged(int status);
    
private:
    Ui::ICPointType *ui;
    QMap<PointType,QString> typeToStr;
    QMap<PointProperty,QString> propertyToStr;

    QMap<QCheckBox*,PointProperty> boxToType;
    QList<QCheckBox*> boxs;
    QCheckBox* _box;

    static ICPointType *instance_;

};

#endif // ICPOINTTYPE_H
