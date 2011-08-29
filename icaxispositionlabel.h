#ifndef ICAXISPOSITIONLABEL_H
#define ICAXISPOSITIONLABEL_H

#include <QLabel>
#include <QStaticText>
//#include <QScopedPointer>

class ICAxisPositionLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ICAxisPositionLabel(QWidget *parent = 0);

    void SetAxisStatusAttribute(const QString & axisName, const QString & measureName);
    void SetAxisPosition(const QString & axisPosition);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *e);

private:
    QStaticText axisName_;
    QString axisPosition_;
    QStaticText measureName_;
//    QScopedPointer<QLabel> measureLabel_;
    QPoint topLeftPoint_;
    QPoint measureTopLeft_;
    QRect posRect_;
};

#endif // ICAXISPOSITIONLABEL_H
