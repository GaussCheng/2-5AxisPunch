#ifndef ICTOUCHSCREENTESTWIDGET_H
#define ICTOUCHSCREENTESTWIDGET_H

#include <QWidget>
#include <QPolygon>

class ICTouchScreenTestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ICTouchScreenTestWidget(QWidget *parent = 0);

    
signals:
    
public slots:
    
    void ClearDraw() {lines_.clear();repaint();}

protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    QPolygon leftPolygon_;
    QPolygon rightPolygon_;

    const static int kLineWidth = 5;

    QList<QPolygon>  lines_;
    QPoint mousePos_;
    bool isPressed_;
};

#endif // ICTOUCHSCREENTESTWIDGET_H
