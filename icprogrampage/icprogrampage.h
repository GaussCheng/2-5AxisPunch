#ifndef ICPROGRAMPAGE_H
#define ICPROGRAMPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <iccheckedbutton.h>
#include <QTableWidgetItem>

#define MAX_POINTS 5
#define COLUMN_COUNTS 9

namespace Ui {
class ICProgramPage;
}

class ICProgramPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit ICProgramPage(QWidget *parent = 0,int pageIndex = 0);
    void setItemNames(QStringList & contents);
    ~ICProgramPage();
    
private slots:
    void itemClicked(QTableWidgetItem*);

private:
    Ui::ICProgramPage *ui;
    QPushButton saveButtons[MAX_POINTS];
    QPushButton testButtons[MAX_POINTS];
    ICCheckedButton reserveButtons[MAX_POINTS - 3];

    QLabel  headers[MAX_POINTS];
    int _index;

};

#endif // ICPROGRAMPAGE_H
