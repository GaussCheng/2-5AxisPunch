#ifndef ICLISTWIDGET_H
#define ICLISTWIDGET_H

#include <QListWidget>

class ICListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ICListWidget(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

signals:

public slots:

};

#endif // ICLISTWIDGET_H
