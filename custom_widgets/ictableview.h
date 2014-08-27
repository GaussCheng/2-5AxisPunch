#ifndef ICTABLEVIEW_H
#define ICTABLEVIEW_H

#include <QTableView>

class ICTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ICTableView(QWidget *parent = 0);

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

};

#endif // ICTABLEVIEW_H
