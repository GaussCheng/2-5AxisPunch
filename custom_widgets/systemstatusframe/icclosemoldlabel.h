#ifndef ICCLOSEMOLDLABEL_H
#define ICCLOSEMOLDLABEL_H

#include "statuslabel.h"
class ICTuneMoldDialog;

class ICCloseMoldLabel : public StatusLabel
{
    Q_OBJECT
public:
    explicit ICCloseMoldLabel(QWidget *parent = 0);
    virtual ~ICCloseMoldLabel();

signals:

public slots:

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
private:
    ICTuneMoldDialog* dialog_;

};

#endif // ICCLOSEMOLDLABEL_H
