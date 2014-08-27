#ifndef ICMESSAGEBOX_H
#define ICMESSAGEBOX_H

#include <QMessageBox>

class ICMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit ICMessageBox(QWidget *parent = 0);

    static int ICWarning ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton );

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent* e);

private:

};

#endif // ICMESSAGEBOX_H
