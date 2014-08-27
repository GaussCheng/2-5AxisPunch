#ifndef ICCOMBOBOX_H
#define ICCOMBOBOX_H

#include <QComboBox>

class ICComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ICComboBox(QWidget *parent = 0);

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

};

#endif // ICCOMBOBOX_H
