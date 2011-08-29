#ifndef ICPAGETAB_H
#define ICPAGETAB_H

#include <QAbstractButton>

class ICPageTab : public QAbstractButton
{
    Q_OBJECT
public:
    explicit ICPageTab(QWidget *parent = 0);

    QSize sizeHint() const { return QSize(67, 34);}
    QSize minimumSizeHint() const { return sizeHint();}

signals:

protected:
    void paintEvent(QPaintEvent *e);
//    void mousePressEvent(QMouseEvent *e);

public slots:

private:
    QPixmap background_;
    QPixmap checkedBackgroud_;

};

#endif // ICPAGETAB_H
