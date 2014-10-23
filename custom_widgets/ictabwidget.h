#ifndef ICTABWIDGET_H
#define ICTABWIDGET_H

#include <QTabWidget>

class ICTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ICTabWidget(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

signals:

public slots:

};

#endif // ICTABWIDGET_H
