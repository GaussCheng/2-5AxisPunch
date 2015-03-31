#ifndef ICPROGRAMPAGE_H
#define ICPROGRAMPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <iccheckedbutton.h>
#include <QTableWidgetItem>
#include <virtualnumerickeypaddialog.h>


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

protected:
    void showEvent(QShowEvent *);
    
signals:
    void returnProgram();

private slots:
    void itemClicked(QTableWidgetItem*);
    void saveButtonsCliked();
    void testButonsClicked();
    void usedButtonsClicked(bool);

    void on_pushButton_clicked();

private:
    Ui::ICProgramPage *ui;
    VirtualNumericKeypadDialog *_dialog;
    QList<QPushButton*> saveButtons;
    QList<QPushButton*> testButtons;
    QList<ICCheckedButton*> reserveButtons;

    QLabel  headers[MAX_POINTS];
    int _index;

};

#endif // ICPROGRAMPAGE_H
