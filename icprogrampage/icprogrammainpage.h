#ifndef ICPROGRAMMAINPAGE_H
#define ICPROGRAMMAINPAGE_H

#include <QWidget>
#include <QPushButton>
#include "iccheckedbutton.h"
#include "icprogrampage.h"

#define MAX_ROWS 8

namespace Ui {
class ICProgramMainPage;
}

class ICProgramMainPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit ICProgramMainPage(QWidget *parent = 0);
    void setVerticalNames(QStringList &contents);
    ~ICProgramMainPage();

protected:
    void showEvent(QShowEvent *);

private slots:
    void usedButtonClicked(bool);
    void programButtonClicked();
    void showMainProgram();

private:
    Ui::ICProgramMainPage *ui;
    QList<ICCheckedButton*> usedButtons;
    QList<QPushButton*> programButtons;
    QList<ICProgramPage*> programPages;

};

#endif // ICPROGRAMMAINPAGE_H
