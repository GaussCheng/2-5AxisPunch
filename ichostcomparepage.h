#ifndef ICHOSTCOMPAREPAGE_H
#define ICHOSTCOMPAREPAGE_H

#include <QDialog>

namespace Ui {
class ICHostComparePage;
}

class ICHostComparePage : public QDialog
{
    Q_OBJECT
    
public:
    explicit ICHostComparePage(QWidget *parent = 0);
    ~ICHostComparePage();
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void on_hostBtn_clicked();

    void on_hmiBtn_clicked();


private:
    Ui::ICHostComparePage *ui;
};

#endif // ICHOSTCOMPAREPAGE_H
