#ifndef ICIOPAGE_H
#define ICIOPAGE_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QBitArray>
#include "iciopoint.h"

class QVBoxLayout;
class QLabel;

class ICIOPage : public QWidget
{
    Q_OBJECT
public:
    enum IOType {Input, Output};
    explicit ICIOPage(QWidget *parent = 0);
    ~ICIOPage();

    IOType PageType() const {return type_;}
    void SetPageType(IOType type) { type_ = type;}

    void BindingIOPoints(const QList<ICIOPoint>& points);

signals:

public slots:
    void UpdateIO();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
private:
    QVBoxLayout* frameLayout_;
    IOType type_;
    QMap<QLabel*, ICIOPoint> ledToPoint_;
    QMap<int, ICIOPoint> backupDescrMap_;
    QBitArray inputs_;
    QBitArray outputs_;
    QList<QLabel*> descrLabels_;
    QList<QLabel*> recsLabels_;
    QList<QLabel*> ioLabels_;

    const QPixmap offPixmap_;
    const QPixmap inputOnPixmap_;
    const QPixmap outputOnPixmap_;

};

#endif // ICIOPAGE_H
