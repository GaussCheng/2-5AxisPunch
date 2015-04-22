#ifndef ICPROGRAMPAGE_H
#define ICPROGRAMPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <iccheckedbutton.h>
#include <QTableWidgetItem>
#include <virtualnumerickeypaddialog.h>
#include "icvirtualhost.h"
#include "icpointtype.h"


#define MAX_POINTS 8
#define AXIS_COUNTS 5
#define MAX_ROWCOUNT 16
#define COLUMN_COUNTS (AXIS_COUNTS + 3)
#define RESERVE_COUNTS (MAX_POINTS - 3)
#define PAGE_MAX_COUNT 8
#define USE_SPACE_ROW  1
#define POINT_SIZE 1
#define ROW_COUNTS ui->tableWidget->rowCount() - USE_SPACE_ROW

namespace Ui {
class ICProgramPage;
}

#define _MoldParam(addr) ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(addr))
#define _SetMoldParam(addr,value) ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(addr),value)


#define _NativeMoldParam(addr) ICMold::CurrentMold()->MoldNativeParam(static_cast<ICMold::ICMoldNativeParam>(addr))
#define _SetNativeMoldParam(addr,value) ICMold::CurrentMold()->SetMoldNativeParam(static_cast<ICMold::ICMoldNativeParam>(addr),value)

typedef struct{
   qint16 x;
   qint16 y;
   qint16 s;
   qint16 r;
   qint16 t;
} ICPoint_;

typedef QSharedPointer<ICPoint_>  PointPtr;
typedef QSharedPointer<ICMoldItem>  MoldItemPtr;


class ICProgramPage : public QWidget
{
    Q_OBJECT

public:
    QList<ICMoldItem> GT_MoldItems();
    QList<ICMoldItem> GT_HeaderItems();
    QList<ICMoldItem> GT_TailMoldItems();

    void refreshCurrentRow(int step);

    QTableWidget * TableWidget();
    static ICProgramPage * Instance_(){
        if(instance_ == NULL)
        {
            instance_ = new ICProgramPage();
        }
        return instance_;
    }

    QList<ICMoldItem> MK_PosItem(int pos);
    PointPtr MK_Point(qint16 x,qint16 y,qint16 s,qint16 r,qint16 t);
    ICMoldItem MK_MoldItem(uint seq,
                            uint num,
                            uint8_t subNum,
                            uint gmVal,
                            uint pos,
                            uint ifVal,
                            uint ifPos,
                            uint sVal,
                            uint dVal,
                            uint sum);

    void InitFixMoldItems();
    void SetPageMaxCount(int count);
    ~ICProgramPage();
public slots:
    void ChangeDelay(int delay);
protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void changeEvent(QEvent *e);
    

signals:
    void returnProgram();

private slots:
    void itemClicked(QTableWidgetItem*);
    void saveButtonsCliked();
    void testButonsClicked();
    void on_pushButton_clicked();
    void on_newButton_clicked();
    void on_modiifyButton_clicked();
    void on_deleteButton_clicked();


    void on_saveButton_clicked();
    void MoldChanged(QString);

    void on_startEdit_clicked(bool checked);

private:
    explicit ICProgramPage(QWidget *parent = 0,int pageIndex = 0,QString pageName = "");
    void InitTableWidget();
    void InitPoint();
    void DisableTestButtons();
    void EnableTestButtons();
    void PreviousPage();
    void NextPage();
    void ReConfigure();

    void DeleteWidgets();
    void InitPointToItem();
    void SaveConfigPoint();
    QList<ICMoldItem> GT_AllMoldItems();
    bool MoldChanged(QList<ICMoldItem>& items);
    bool MoldConfigChanged();

    void GT_CalculateItem(QList<ICMoldItem>& items);

private:
    Ui::ICProgramPage *ui;
    VirtualNumericKeypadDialog *_dialog;
    ICPointType *_typeDialog;
    QList<QPushButton*> saveButtons;
    QList<QPushButton*> testButtons;
    QList<PointType> pointTypes;
    QMap<PointType,QList<ICMoldItem> > pointToItem;
    QStringList standPrograms_;
    QList<QIntValidator *> validators_;
    static ICProgramPage * instance_;

    ICVirtualHost *_host;
    int _index;
    QString _pageName;
    int _pageMaxCount;
    int _currentPage;

    QList<PointPtr> allPoints;
    QList<ICMoldItem> allItems;


    ICMoldItem waitM10;
    ICMoldItem outY37On;
    ICMoldItem outM11;
    ICMoldItem waitM12;
    ICMoldItem outY37Off;
    ICMoldItem outY31On;
    ICMoldItem outY31Off;
    ICMoldItem outM27On;
    ICMoldItem waitM14;
    ICMoldItem outPermit;




};

#endif // ICPROGRAMPAGE_H
