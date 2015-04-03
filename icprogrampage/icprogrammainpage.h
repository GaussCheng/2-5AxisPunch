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
    void hideEvent(QHideEvent *);

private slots:
    void usedButtonClicked(bool);
    void programButtonClicked();
    void showMainProgram();
private:
    QList<ICMoldItem> GT_AllItems();
    void GT_CalculateItem(QList<ICMoldItem> items);
    bool MoldChanged();


private:
    Ui::ICProgramMainPage *ui;
    QList<ICCheckedButton*> usedButtons;
    QList<QPushButton*> programButtons;
    QList<ICProgramPage*> programPages;
    int oldUsed;

};

#endif // ICPROGRAMMAINPAGE_H
