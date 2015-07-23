#ifndef ICCOMBOBOX_H
#define ICCOMBOBOX_H

#include <QComboBox>

class ICComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ICComboBox(QWidget *parent = 0);

    int LastValue() const { return oldValue_;}

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void focusInEvent(QFocusEvent *e);


    int oldValue_;

};

#endif // ICCOMBOBOX_H
