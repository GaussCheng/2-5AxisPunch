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
   Get_Finish,
   Put_Wait,
   Put_Up,
   Put,
   Put_Finish,
   Reserve,
   NULL_Type = -1

} ;

class ICPointType : public QDialog
{
    Q_OBJECT


public:
    explicit ICPointType(QWidget *parent = 0);
    ~ICPointType();
    QString toString(PointType type = NULL_Type);
    PointType currentType();

protected:
    void changeEvent(QEvent *);
private:
    void Init_();
private slots:
    void stateChanged(int status);
    
private:
    Ui::ICPointType *ui;
    QMap<PointType,QString> typeToStr;
    QMap<QCheckBox*,PointType> boxToType;
    QCheckBox * _box;
};

#endif // ICPOINTTYPE_H
