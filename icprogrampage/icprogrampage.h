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
//#include "icpointpproperty.h"


#define MAX_POINTS 8
#define AXIS_COUNTS 5
#define MAX_ROWCOUNT 20
#define COLUMN_COUNTS (AXIS_COUNTS + 1)
#define RESERVE_COUNTS (MAX_POINTS - 3)
#define PAGE_MAX_COUNT 8
#define USE_SPACE_ROW  0
#define POINT_SIZE 2
#define TOTOAL_POINT_SIZE 1

#define ROW_COUNTS ui->tableWidget->rowCount() - USE_SPACE_ROW
#define ROW_HIGHT 38

namespace Ui {
class ICProgramPage;
}

#define _MoldParam(addr) ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(addr))
#define _SetMoldParam(addr,value) ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(addr),value)
#define _LOW16_INT32(v)  (v & 0xFFFF)
#define _HIGH16_INT32(v) ((v >> 16) & 0xFFFF)

#define _NativeMoldParam(addr) ICMold::CurrentMold()->MoldNativeParam(static_cast<ICMold::ICMoldNativeParam>(addr))
#define _SetNativeMoldParam(addr,value) ICMold::CurrentMold()->SetMoldNativeParam(static_cast<ICMold::ICMoldNativeParam>(addr),value)



typedef struct{
   qint32 x;
   qint32 y;
   qint32 s;
   qint32 r;
   qint32 t;
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

    QList<ICMoldItem> MK_PosItem(int row,int pos);
    PointPtr MK_Point(quint32 x,quint32 y,quint32 s,quint32 r,quint32 t);
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



protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void changeEvent(QEvent *e);
    void timerEvent(QTimerEvent *);

signals:
    void returnProgram();

private slots:
    void itemClicked(QTableWidgetItem*);
    void saveButtonsCliked();
    void testButonsPressed();
    void testButonsReleased();

    void on_newButton_clicked();
    void on_modiifyButton_clicked();
    void on_deleteButton_clicked();


    void on_saveButton_clicked();
    void MoldChanged(QString);

    void on_startEdit_clicked(bool checked);

    void on_seveoEdit_toggled(bool checked);

    void on_manualButton_pressed();

    void on_manualButton_released();

private:
    explicit ICProgramPage(QWidget *parent = 0,int pageIndex = 0,QString pageName = "");
    void InitTableWidget();
    void InitPoint();
    void DisableTestButtons();
    void EnableTestButtons();
    void PreviousPage();
    void NextPage();
    void ReConfigure();
    int GT_PointCount();
    int GT_PointIndexFromRow(int row);
    quint32 GetPointValue(quint16 pos);
    qint32 GetPosValue(ICVirtualHost::ICStatus status);



    void DeleteWidgets();
    void InitPointToItem();
    void SaveConfigPoint();
    QList<ICMoldItem> GT_AllMoldItems();

    bool MoldChanged(QList<ICMoldItem>& items);
    bool PointConfigChanged(QList<ICPointConfig>& items);

    bool MoldConfigChanged();
    bool LastIsSmooth();
    void GT_CalculateItem(QList<ICMoldItem>& items);
    void SetRowSMooth(int index,bool s);

    quint32 GT_LastAxisNum();

private:
    Ui::ICProgramPage *ui;
    VirtualNumericKeypadDialog *_dialog;
    ICPointType *_typeDialog;
    QList<QPushButton*> saveButtons;
    QList<QPushButton*> testButtons;
    QList<PointType> pointTypes;
    QList<ICPointConfig> pointConfigs;
    QMap<PointType,QList<ICMoldItem> > pointToItem;
    QMap<PointProperty,ICMoldItem > propertyToItem;

    QStringList standPrograms_;
    QList<QIntValidator *> validators_;
    QIntValidator *dValidator;
    static ICProgramPage * instance_;

    ICVirtualHost *_host;
    int _index;
    QString _pageName;
    int _pageMaxCount;
    int _currentPage;

    QList<PointPtr> allPoints;
    QList<ICMoldItem> allItems;


    ICMoldItem waitM10;
    ICMoldItem outM11;
    ICMoldItem waitM12;
    ICMoldItem outPermit;
    ICMoldItem checkM15;


    ICMoldItem outY37On;  //输出
    ICMoldItem outY37Off;
    ICMoldItem outY40On;
    ICMoldItem outY40Off;
    ICMoldItem outY22On;
    ICMoldItem outY22Off;
    ICMoldItem outY23On;
    ICMoldItem outY23Off;

    ICMoldItem waitX41;  //等待
    ICMoldItem waitX42;
    ICMoldItem waitX43;
    ICMoldItem waitX44;

    ICMoldItem outPermit2;


    //遗弃
    ICMoldItem outM27On;
    ICMoldItem waitM14;
    int timerId;

    QString oriStyle;
    QString yOnStyle;

};

#endif // ICPROGRAMPAGE_H
