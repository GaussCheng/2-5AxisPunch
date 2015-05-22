#ifndef ICTABLEWIDGET_H
#define ICTABLEWIDGET_H

#include <QTableWidget>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit ICTableWidget(QWidget *parent = 0);
    ~ICTableWidget();
protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};

#endif // ICTABLEWIDGET_H
