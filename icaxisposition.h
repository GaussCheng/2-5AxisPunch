#ifndef ICAXISPOSITION_H
#define ICAXISPOSITION_H

#include <QLabel>

class ICAxisPosition : public QLabel
{
    Q_OBJECT
public:
    explicit ICAxisPosition(QWidget *parent = 0);

    inline void SetAxisName(const QString & axisName) { axisName_ = axisName; }

signals:

public slots:

private:
    QString axisName_;
};

#endif // ICAXISPOSITION_H
