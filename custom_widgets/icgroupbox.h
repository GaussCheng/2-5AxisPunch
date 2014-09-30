#ifndef ICGROUPBOX_H
#define ICGROUPBOX_H

#include <QGroupBox>

class ICGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit ICGroupBox(QWidget *parent = 0);


protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

signals:

public slots:

};

#endif // ICGROUPBOX_H
