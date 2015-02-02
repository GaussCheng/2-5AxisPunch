#ifndef ICPLAINTEXTEDIT_H
#define ICPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class ICPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ICPlainTextEdit(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *e);
signals:

public slots:

};

#endif // ICPLAINTEXTEDIT_H
