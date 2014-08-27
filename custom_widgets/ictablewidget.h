#ifndef ICTABLEWIDGET_H
#define ICTABLEWIDGET_H

#include <QTableWidget>

class ICTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit ICTableWidget(QWidget *parent = 0);

protected:

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
signals:

public slots:

};

#endif // ICTABLEWIDGET_H
