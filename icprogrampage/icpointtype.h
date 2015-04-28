#ifndef ICPOINTTYPE_H
#define ICPOINTTYPE_H

#include <QDialog>
#include <QCheckBox>
#include <QMap>


namespace Ui {
class ICPointType;
}


enum PointType{
    Get_Wait,
    Get_Up,
    Get,
    Get_Wait2,
    Put_Wait,
    Put_Up,
    Put,
    Put_Wait2,
    Reserve,

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
    PointType currentType();


protected:
    void changeEvent(QEvent *);
private:
    explicit ICPointType(QWidget *parent = 0);
private slots:
    void stateChanged(int status);
    
private:
    Ui::ICPointType *ui;
    QMap<PointType,QString> typeToStr;
    QMap<QCheckBox*,PointType> boxToType;
    QCheckBox * _box;
    static ICPointType *instance_;

};

#endif // ICPOINTTYPE_H
