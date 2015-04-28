#ifndef ICPOINTPPROPERTY_H
#define ICPOINTPPROPERTY_H

#include <QWidget>

namespace Ui {
class ICPointpProperty;
}

class ICPointpProperty : public QWidget
{
    Q_OBJECT
    
public:
    explicit ICPointpProperty(QWidget *parent = 0);
    ~ICPointpProperty();
    
private:
    Ui::ICPointpProperty *ui;
};

#endif // ICPOINTPPROPERTY_H
