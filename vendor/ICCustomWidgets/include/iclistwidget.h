#ifndef ICLISTWIDGET_H
#define ICLISTWIDGET_H

#include <QListWidget>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ICListWidget(QWidget *parent = 0);
    ~ICListWidget();
protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};

#endif // ICLISTWIDGET_H
