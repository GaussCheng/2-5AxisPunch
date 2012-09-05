#ifndef ICMACHINEFIGURE_H
#define ICMACHINEFIGURE_H

#include <QWidget>

class ICLineEditWrapper;

namespace Ui {
class ICMachineFigure;
}

class ICMachineFigure : public QWidget
{
    Q_OBJECT
    
public:
    explicit ICMachineFigure(QWidget *parent = 0);
    ~ICMachineFigure();
    
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    
private slots:
    void on_saveButton_clicked();

private:
    Ui::ICMachineFigure *ui;
    QList<ICLineEditWrapper*> wrappers_;
};

#endif // ICMACHINEFIGURE_H
