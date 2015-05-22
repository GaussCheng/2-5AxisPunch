#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QWidget>
#include <QLayout>
#include <QSqlRecord>
#include <QSqlQuery>

#include "tst_testiccustomwidgets.h"
#include "icalarmbar.h"
#include "icbarrel.h"
#include "icbutton.h"
#include "iccheckbox.h"
#include "icclocklabel.h"
#include "iccombobox.h"
#include "icdoublespinbox.h"
#include "icframebutton.h"
#include "icgroupbox.h"
#include "iclabel.h"
#include "iclineedit.h"
#include "icslidingwidget.h"
#include "icspinbox.h"
#include "icstatuslabel.h"
#include "ictableview.h"
#include "ictwostatuslabel.h"
#include "icunitlabel.h"
#include "icrecordtablemodel.h"
#include "icupdatepackmodel.h"
#include "icalarmtablemodel.h"
#include "ictimescaleplot.h"
#include "icsqltableview.h"

#define BARREL
//#define LINEEDIT
//#define CHECKBOX
//#define SPINBOX
//#define WAIT1000
#define CURRENLINE qDebug()<<(QString)__FILE__<<"line:"<<__LINE__;

Q_DECLARE_METATYPE(ICCheckBox*);
Q_DECLARE_METATYPE(ICSpinBox*);
Q_DECLARE_METATYPE(QList<Qt::Key>);
Q_DECLARE_METATYPE(Qt::Orientation);

TempWidget::TempWidget(QWidget *parent) :
        QWidget(parent)
{
    checkbox_1 = new ICCheckBox();
    checkbox_2 = new ICCheckBox();
    checkbox_3 = new ICCheckBox();
    checkbox_1->setText("checkbox_1");
    checkbox_2->setText("checkbox_2");
    checkbox_3->setText("checkbox_3");
    gridlayout = new QGridLayout;
    gridlayout->addWidget(checkbox_1);
    gridlayout->addWidget(checkbox_2);
    gridlayout->addWidget(checkbox_3);
    setLayout(gridlayout);
}

TempWidget::~TempWidget()
{
    delete checkbox_1;
    delete checkbox_2;
    delete checkbox_3;
    delete gridlayout;
}

TestComboboxWidget::TestComboboxWidget(QWidget *parent) :
        QWidget(parent)
{
    combobox_1 = new ICComboBox(this);
    combobox_2 = new ICComboBox(this);
    combobox_3 = new ICComboBox(this);
    gridlayout = new QGridLayout;
    gridlayout->addWidget(combobox_1);
    gridlayout->addWidget(combobox_2);
    gridlayout->addWidget(combobox_3);
    setLayout(gridlayout);
    setFocusPolicy(Qt::StrongFocus);
}

TestComboboxWidget::~TestComboboxWidget()
{
    delete combobox_1;
    delete combobox_2;
    delete combobox_3;
    delete gridlayout;
}

TestSpinboxWidget::TestSpinboxWidget(QWidget *parent) :
        QWidget(parent)
{
    spinbox_1 = new ICSpinBox;
    spinbox_2 = new ICSpinBox;
    spinbox_3 = new ICSpinBox;
    gridlayout = new QGridLayout;
    gridlayout->addWidget(spinbox_1);
    gridlayout->addWidget(spinbox_2);
    gridlayout->addWidget(spinbox_3);
    setLayout(gridlayout);
}
TestSpinboxWidget::~TestSpinboxWidget()
{
    delete spinbox_1;
    delete spinbox_2;
    delete spinbox_3;
    delete gridlayout;
}


class TestICCustomWidgets : public QObject
{
    Q_OBJECT

public:
    TestICCustomWidgets();

signals:
    void alarmbarSignal();
    void unitlabelSignal(QString);
    void twostatuslabelSignal();

public slots:
    void buttonSlot(){button->setText("clicked");}
    void recordtablemodelSlot();
    void updatepackmodelSlot();

    ////////////////////////////////////////////////////
private:
    ICAlarmBar *alarmbar;
    ICBarrel *barrel;
    ICButton *button;
    ICLabel *label;
    ICStatusLabel *statuslabel;
    ICLineEdit *lineedit;
    ICUnitLabel *unitlabel;
    ICTwoStatusLabel *twostatuslabel;
    ICTableView *tableview;
    TempWidget * checkboxwidget;    //ICCheckBox
    ICClockLabel *clocklabel;
    TestComboboxWidget *combobox;
    ICDoubleSpinBox *doublespinbox;
    ICFrameButton *framebutton;
    ICGroupBox *groupbox;
    TestSpinboxWidget *spinbox;
    ICRecordTableModel *recordtablemodel;
    bool isreceiverecordtablemodelsignal;
    ICUpdatePackModel *updatepackmodel;
    bool isupdatepackmodelsignal;
    ICAlarmTableModel *alarmtablemodel;
    QSqlDatabase db;
    QSqlDatabase testDB;

    ////////////////////////////////////////////////////
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testICAlarmbar();
    void testICAlarmbar_data();
    void testICBarrel();
    void testICBarrel_data();
    void testICButton();
    void testICLabel();
    void testICLabel_data();
    void testICStatuslabel();
    void testICStatuslabel_data();
    void testICLineEdit();
    void testICLineEdit_data();
    void testICLineEditevent();
    void testICLineEditevent_data();
    void testICSlidingWidget();
    void testICUnitLabel();
    void testICUnitLabel_data();
    void testICTwoStatusLabel();
    void testICTwoStatusLabel_data();
    void testICTableView_SelectedRowsIndex();
    void testICTableView_SelectedRowsIndex_data();
    void testICCheckBox();
    void testICCheckBox_data();
    void testICClockLabel();
    void testICComboBox();
    void testICDoubleSpinBox();
    void testICDoubleSpinBox_data();
    void testICFrameButton();
    void testICGroupBox();
    void testICGroupBox_data();
    void testICSpinBox();
    void testICSpinBox_data();
    void testICRecordTableModel();
    void testICRecordTableModel_data();
    void testICUpdatePackModel();
    void testICUpdatePackModel_data();
    void testICAlarmTableModel();
    void testICAlarmTableModel_data();

    void testICTimeScalePlot_Construct();
    void testICTimeScalePlot_SetXAxisTitle();
    void testICTimeScalePlot_SetYAxisTitle();
    void testICTimeScalePlot_SetYAxisScale();

    void testICSqlTableView_Construct();
    void testICSqlTableView_SetSqlTableModel();
    void testICSqlTableView_SetTableName();
    void testICSqlTableView_InsertRecord();
    void testICSqlTableView_ClearRecord();
    void testICSqlTableView_SetSort();
    void testIcSqlTableView_SetHeaderData();
};

TestICCustomWidgets::TestICCustomWidgets()
{

}

void TestICCustomWidgets::initTestCase()
{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("ICCustomWidgetTestDatabase");
//    if(!db.isValid())
//    {
//        qCritical("Open Database fail!!");
//    }
//    if(!db.open())
//    {
//        qCritical("Open Database fail!!");
//    }
    alarmbar = new ICAlarmBar;
    connect(this,SIGNAL(alarmbarSignal()),alarmbar,SLOT(ShowNextAlarm()));
    barrel = new ICBarrel;
    button = new ICButton;
    connect(button,SIGNAL(clicked()),this,SLOT(buttonSlot()));
    label = new ICLabel;
    statuslabel = new ICStatusLabel;
    lineedit = new ICLineEdit;
    lineedit->setMinimumWidth(300);
    //    lineedit->show();
    //    QTest::qWaitForWindowShown(lineedit);
    unitlabel = new ICUnitLabel;
    connect(this,SIGNAL(unitlabelSignal(QString)),unitlabel,SLOT(setSuffix(QString)));
    twostatuslabel = new ICTwoStatusLabel;
    connect(this,SIGNAL(twostatuslabelSignal()),twostatuslabel,SLOT(OnTimerOut()));
    tableview = new ICTableView;
    checkboxwidget = new TempWidget;    //ICCheckBox
    //    checkboxwidget->show();
    //    QTest::qWaitForWindowShown(checkboxwidget);
    clocklabel = new ICClockLabel;
    combobox = new TestComboboxWidget;
    //    combobox->show();
    //    QTest::qWaitForWindowShown(combobox);
    doublespinbox = new ICDoubleSpinBox;
    framebutton = new ICFrameButton;
    groupbox = new ICGroupBox;
    spinbox = new TestSpinboxWidget;
    //    spinbox->show();
    //    QTest::qWaitForWindowShown(spinbox);
    recordtablemodel = new ICRecordTableModel(QDir("/home/zzb/windowshare/"));
    connect(recordtablemodel,SIGNAL(headerDataChanged(Qt::Orientation,int,int)),SLOT(recordtablemodelSlot()));
    updatepackmodel = new ICUpdatePackModel(QDir("/home/zzb/windowshare/"));
    connect(updatepackmodel,SIGNAL(headerDataChanged(Qt::Orientation,int,int)),SLOT(updatepackmodelSlot()));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("InjectionMachineDatabase");
    db.open();
    alarmtablemodel = new ICAlarmTableModel;
    alarmtablemodel->setTable("tb_alarmrecord");
    alarmtablemodel->select();

    testDB = QSqlDatabase::addDatabase("QSQLITE");
    testDB.setDatabaseName("ICCustomWidgetTestDatabase");
    testDB.open();
}

////////////////////////////////////////////////////
void TestICCustomWidgets::cleanupTestCase()
{
#ifdef WAIT1000
    QTest::qWait(1000000);
#endif
    delete alarmbar;
    delete barrel;
    delete button;
    delete label;
    delete statuslabel;
    delete lineedit;
    delete unitlabel;
    delete twostatuslabel;
    delete tableview;
    delete checkboxwidget;     //ICCheckBox
    delete clocklabel;
    delete combobox;
    delete doublespinbox;
    delete framebutton;
    delete groupbox;
    delete spinbox;
    delete recordtablemodel;
    delete updatepackmodel;
    delete alarmtablemodel;

    QSqlQuery query("delete from testtable", testDB);
    query.exec();
}

void TestICCustomWidgets::testICAlarmbar()
{
    int i=0;
    QFETCH(QStringList,stringList);
    alarmbar->SetAlarmStrings(stringList);

    if(stringList.isEmpty())
    {
        emit alarmbarSignal();
        QCOMPARE(alarmbar->text().isEmpty(),true);
        return;
    }
    do{
        emit alarmbarSignal();
        QCOMPARE(alarmbar->text(),stringList.at(i));
        i++;
    }
    while(stringList.size()>i);
    emit alarmbarSignal();
    QCOMPARE(alarmbar->text(),stringList.at(0));
}

void TestICCustomWidgets::testICAlarmbar_data()
{
    QTest::addColumn<QStringList>("stringList");

    QStringList emptyStringList,stringList1,stringList2;
    stringList1 << "aa";
    stringList2 << "A" << "bb" <<"ccc" << "daad" <<""<<"**"<<"123"<<" #b*1";
    QTest::newRow("alarmbarRow0") << emptyStringList;
    QTest::newRow("alarmbarRow1") << stringList1;
    QTest::newRow("alarmbarRow2") <<stringList2;
}

void TestICCustomWidgets::testICBarrel()
{
    QFETCH(int,bareelcount);
    QFETCH(int,expectbareelcount);
    QFETCH(int,barrelvalueindex);
    QFETCH(double,barrelvalue);
    QFETCH(double,expectbarrelvalue);
    QFETCH(int,barrelspecialtextindex);
    QFETCH(QString,barrelspecialtext);
    QFETCH(QString,expectbarrelspecialtext);

    barrel->SetBarrelCount(bareelcount);
    QCOMPARE(barrel->BarrelCount(),expectbareelcount);
    barrel->SetBarrelValue(barrelvalueindex,barrelvalue);
    QCOMPARE(barrel->BarrelValue(barrelvalueindex),expectbarrelvalue);
    barrel->BindSpeicalValueText(barrelvalueindex,barrelspecialtextindex,barrelspecialtext);
    //    QCOMPARE(barrel->"text",expectbarrelspecialtext);
}

void TestICCustomWidgets::testICBarrel_data()
{
    QTest::addColumn<int>("bareelcount");           //1
    QTest::addColumn<int>("expectbareelcount");     //2
    QTest::addColumn<int>("barrelvalueindex");      //3
    QTest::addColumn<double>("barrelvalue");           //4
    QTest::addColumn<double>("expectbarrelvalue");     //5
    QTest::addColumn<int>("barrelspecialtextindex");//6
    QTest::addColumn<QString>("barrelspecialtext"); //7
    QTest::addColumn<QString>("expectbarrelspecialtext");//8

    /* expectbareelcount = (bareelcount < 2)? 2 : bareelcount */
    /* expectbarrelvalue = (barrelvalueindex < expectbareelcount)? barrelvalue : 0  */

    /***************************1********2********3******4*******5*******6*******7*******************8********/
    //    QTest::newRow("barrelRow0")<<0      <<2     <<-1    <<2     <<0     <<-1    <<"negative"        <<"";
    QTest::newRow("barrelRow1")<<-1     <<0     <<0     <<0.0     <<0.0     <<10    <<"largethancount"  <<"";
#ifdef BARREL
    QTest::newRow("barrelRow2")<<2      <<2     <<1     <<2.0     <<2.0     <<0     <<"normaltext"      <<"normaltext";
    QTest::newRow("barrelRow3")<<3      <<3     <<2     <<10.0    <<10.0    <<2     <<"equalcountindex" <<"";
    QTest::newRow("barrelRow4")<<5      <<5     <<5     <<30.0    <<0.0     <<1     <<"indextoolarge"   <<"";
    QTest::newRow("barrelRow5")<<8      <<8     <<7     <<111.0   <<111.0   <<7     <<"123"             <<"123";
    QTest::newRow("barrelRow6")<<30     <<30    <<29    <<146.0   <<146.0   <<0     <<"normaltext"      <<"normaltext";
    QTest::newRow("barrelRow7")<<30     <<30    <<30    <<146.0   <<0.0     <<0     <<""                <<"";
    QTest::newRow("barrelRow8")<<30     <<30    <<31    <<146.0   <<0.0     <<0     <<""                <<"";
    QTest::newRow("barrelRow8")<<30     <<30    <<300   <<146.0   <<0.0     <<0     <<""                <<"";
#endif
}


void TestICCustomWidgets::testICButton()
{
    QString buttontext = button->text();
    QTest::keyPress(button,Qt::Key_Return);
    QCOMPARE(button->text(),(QString)"clicked");

    button->setText("");
    QTest::keyPress(button,Qt::Key_1);
    QCOMPARE(button->text(),(QString)"");
    button->setText("@#-*1asA2");
    QTest::keyPress(button,Qt::Key_A);
    QCOMPARE(button->text(),(QString)"@#-*1asA2");

    button->setText("@#-*1asA2");
    QTest::keyPress(button,Qt::Key_Return);
    QCOMPARE(button->text(),(QString)"clicked");

    button->setText(buttontext);
    QCOMPARE(button->text(),buttontext);
}

void TestICCustomWidgets::testICLabel()
{
    QFETCH(QString,specialtext);
    QFETCH(QString,changetext);
    QFETCH(bool,labelisenable);

    label->SetSpecialText(specialtext);
    label->SetEnSpecialText(labelisenable);
    label->ChangeText(changetext);
    if(labelisenable)
        QCOMPARE(label->text(),specialtext);
    else
        QCOMPARE(label->text(),changetext);
}

void TestICCustomWidgets::testICLabel_data()
{
    QTest::addColumn<QString>("specialtext");
    QTest::addColumn<QString>("changetext");
    QTest::addColumn<bool>("labelisenable");

    QTest::newRow("allnull")<<""<<""<<false;
    QTest::newRow("allnull")<<""<<""<<true;
    QTest::newRow("allnull")<<"***"<<""<<false;
    QTest::newRow("allnull")<<"***"<<""<<true;
    QTest::newRow("allnull")<<""<<"2"<<false;
    QTest::newRow("allnull")<<""<<"1"<<true;
    QTest::newRow("allnull")<<"*"<<"**"<<false;
    QTest::newRow("allnull")<<"*"<<"**"<<true;
    QTest::newRow("allnull")<<"specialtext"<<"changetext"<<false;
    QTest::newRow("allnull")<<"specialtext"<<"changetext"<<true;
}

void TestICCustomWidgets::testICStatuslabel()
{
    QFETCH(QString,setTextString);
    statuslabel->SetText(setTextString);
    QCOMPARE(statuslabel->Text(),setTextString);

    QFETCH(QString,setUnitTextString);
    statuslabel->SetUnitText(setUnitTextString);
    QCOMPARE(statuslabel->UnitText(),setUnitTextString);

    QFETCH(QString,setSpecialTextString);
    statuslabel->SetSpecialText(setSpecialTextString);
    QCOMPARE(statuslabel->SpecialText(),setSpecialTextString);


    statuslabel->SetText(setTextString);
    statuslabel->SetSpecialText(setSpecialTextString);
    statuslabel->SetEnSpecialText(true);
    QCOMPARE(statuslabel->Text(),setSpecialTextString);
    statuslabel->SetEnSpecialText(false);
    QCOMPARE(statuslabel->Text(),setTextString);

}

void TestICCustomWidgets::testICStatuslabel_data()
{
    QTest::addColumn<QString>("setTextString");
    QTest::addColumn<QString>("setUnitTextString");
    QTest::addColumn<QString>("setSpecialTextString");
    QString setTextString0("a");
    QString const setUnitTextString0("a");
    QString const setSpecialTextString0("a");
    QTest::newRow("setTextStringRow0") << setTextString0 << setUnitTextString0 << setSpecialTextString0;
}

void TestICCustomWidgets::testICLineEdit()
{
    QBENCHMARK{
        ICLineEdit *t = new ICLineEdit();
        delete t;
    }
    QFETCH(int,setDecimalsInt);
    QFETCH(double, doubleInput);
    QFETCH(QString, suffix);
    QFETCH(QString, stringResult);
    QFETCH(double, max);
    QFETCH(double, min);
    QFETCH(double, lastValue);

    lineedit->setStyleSheet(    "border: 1px solid;"
            "background-color:white;"
            "border-left-color: lightgray;"
            "border-top-color: lightgray;"
            "border-right-color: white;"
            "border-bottom-color: white;"
            "border-radius: 5px;"
            "font-size:18px;"
            "selection-background-color:  rgb(246, 153, 63);");
    lineedit->SetSuffix(suffix);
    lineedit->SetDecimals(setDecimalsInt);
    lineedit->SetMaximum(max);
    lineedit->SetMinimum(min);
    lineedit->SetValue(doubleInput);

    QCOMPARE(lineedit->LastValue(), lastValue);
    QCOMPARE(lineedit->Decimals(),setDecimalsInt < 0 ? 0 : setDecimalsInt);
    QCOMPARE(lineedit->text(), stringResult);

    ICLineEdit edit;
    edit.setText("100");
    edit.SetRange(0, 600);
    QTestEventList events;
    events.addKeyPress('7');
    events.addKeyPress('0');
    events.addKeyPress('0');
    events.addKeyPress(Qt::Key_Return);
    events.simulate(&edit);
    QCOMPARE(edit.Value(), 70.0);
}

void TestICCustomWidgets::testICLineEdit_data()
{
    QTest::addColumn<double>("doubleInput");//1
    QTest::addColumn<int>("intInput");      //2
    QTest::addColumn<double>("max");        //3
    QTest::addColumn<double>("min");        //4
    QTest::addColumn<QString>("suffix");    //5
    QTest::addColumn<int>("setDecimalsInt");//6
    QTest::addColumn<QString>("stringResult");//7
    QTest::addColumn<double>("lastValue");    //8


    /**********************************************1******2*******3*******4*******5**********6********7***********8******/
    QTest::newRow("init test")            <<double(0)   <<0     <<0.0   <<0.0   <<""        <<0     <<"0"       <<-1.0;
#ifdef LINEEDIT
    QTest::newRow("Has Suffix")           <<double(1)   <<1     <<1.0   <<1.0   <<"mm"      <<0     <<"1mm"     <<0.0;
    QTest::newRow("negative Decimals")    <<double(0)   <<0     <<0.0   <<0.0   <<""        <<-1    <<"0"       <<1.0;
    QTest::newRow("Int Has Decimals")     <<double(123) <<123   <<200.0 <<100.0 <<""        <<2     <<"123.00"  <<0.0;
    QTest::newRow("Double Has Decimals")  <<123.123     <<123   <<200.0 <<100.0 <<""        <<3     <<"123.123" <<123.0;
    QTest::newRow("Input Precision is higher")<<12.123  <<123   <<200.0 <<0.0   <<""        <<2     <<"12.12"   <<123.123;
#endif
}

void TestICCustomWidgets::testICLineEditevent()
{
    QTestEventList events;
    QString prelineedittext;
    QFETCH(double,maximum);
    QFETCH(double,minimum);
    QFETCH(QString,suffix);
    QFETCH(int,decimals);
    QFETCH(int,specialvalue);
    QFETCH(QString,specialvaluetext);
    QFETCH(bool,onlynumeric);
    QFETCH(QList<Qt::Key>,inputkey);
    QFETCH(QString,expecttext);

    lineedit->SetMaximum(maximum);
    lineedit->SetMinimum(minimum);
    lineedit->SetSuffix(suffix);
    lineedit->SetDecimals(decimals);
    lineedit->SetSpecialValue(specialvalue);
    lineedit->SetSpecialValueText(specialvaluetext);
    lineedit->SetOnlyNumeric(onlynumeric);

    lineedit->show();
    QTest::qWaitForWindowShown(lineedit);
    lineedit->setFocus();
    QCOMPARE(lineedit->hasFocus(),true);

    if(lineedit->hasFocus())
    {
        lineedit->selectAll();
        prelineedittext = lineedit->text();
        for(int i=0;i<inputkey.size();i++)
            events.addKeyPress(inputkey.at(i));
        events.addKeyPress(Qt::Key_Return);
        events.simulate(lineedit);

        QCOMPARE(lineedit->Suffix(),suffix);
        QCOMPARE(lineedit->Decimals(),decimals);
        QCOMPARE(lineedit->SpecialValue(),specialvalue);
        QCOMPARE(lineedit->SpecialValueText(),specialvaluetext);
        QCOMPARE(lineedit->IsOnlyNumeric(),onlynumeric);

//        CURRENLINE;
//        qDebug()<<"lineedit->text()="<<lineedit->text();//lineedit->text()必须等于如下三个之一
//        qDebug()<<"prelineedittext="<<prelineedittext;
//        qDebug()<<"specialvaluetext+suffix="<<specialvaluetext+suffix;
//        qDebug()<<"expecttext="<<expecttext;

        if(onlynumeric)
        {
            if(lineedit->text() != prelineedittext)
            {
                if(lineedit->text() != specialvaluetext+suffix)
                    QCOMPARE(lineedit->text(),expecttext);
            }
        }
        else
        {
            QCOMPARE(lineedit->text(),expecttext);
        }
    }
}

void TestICCustomWidgets::testICLineEditevent_data()
{
    QTest::addColumn<double>("maximum");    //1
    QTest::addColumn<double>("minimum");    //2
    QTest::addColumn<QString>("suffix");    //3
    QTest::addColumn<int>("decimals");      //4
    QTest::addColumn<int>("specialvalue");  //5
    QTest::addColumn<QString>("specialvaluetext");  //6
    QTest::addColumn<bool>("onlynumeric");          //7
    QTest::addColumn<QList<Qt::Key> >("inputkey");  //8
    QTest::addColumn<QString>("expecttext");        //9 //如果7:onlynumeric==false,那么expecttext不会在程序中引用;

    QList<Qt::Key> keylist0, keylist1, keylist2, keylist3, keylist4, keylist5, keylist6, keylist7, keylist8;
    keylist0 <<Qt::Key_A<<Qt::Key_0<<Qt::Key_1<<Qt::Key_2<<Qt::Key_3<<Qt::Key_Return;
    keylist1 <<Qt::Key_A<<Qt::Key_B<<Qt::Key_C<<Qt::Key_D<<Qt::Key_E<<Qt::Key_F<<Qt::Key_G;
    keylist2 <<Qt::Key_0<<Qt::Key_0<<Qt::Key_0<<Qt::Key_0<<Qt::Key_0<<Qt::Key_0<<Qt::Key_0;
    keylist3 <<Qt::Key_1<<Qt::Key_1<<Qt::Key_1<<Qt::Key_1<<Qt::Key_1<<Qt::Key_1<<Qt::Key_1;
    keylist4 <<Qt::Key_9<<Qt::Key_9<<Qt::Key_9<<Qt::Key_9<<Qt::Key_9<<Qt::Key_9<<Qt::Key_9;
    keylist5 <<Qt::Key_Backspace<<Qt::Key_Backspace<<Qt::Key_Backspace<<Qt::Key_Backspace<<Qt::Key_Backspace;
    keylist6 <<Qt::Key_A<<Qt::Key_B<<Qt::Key_C<<Qt::Key_D<<Qt::Key_E<<Qt::Key_F<<Qt::Key_G;
    keylist7 <<Qt::Key_1<<Qt::Key_Backspace<<Qt::Key_2<<Qt::Key_Backspace<<Qt::Key_3<<Qt::Key_Backspace<<Qt::Key_4;
    keylist8 <<Qt::Key_1<<Qt::Key_2<<Qt::Key_3<<Qt::Key_4<<Qt::Key_5<<Qt::Key_6<<Qt::Key_7
            <<Qt::Key_8<<Qt::Key_9<<Qt::Key_1<<Qt::Key_2<<Qt::Key_3<<Qt::Key_4;


    /*************************************1*******2**********3********4****5******6********7********8***********9*********/
    QTest::newRow("lineediteventRow0")<<100.0  <<0.0   <<" suffix0"  <<2  <<0  <<"***"  <<true <<keylist0<<"1.23 suffix0";
#ifdef LINEEDIT
    QTest::newRow("lineediteventRow1")<<100.0  <<0.0   <<" suffix1"  <<2  <<0  <<"***"  <<true <<keylist1<<"prelineedittext";
    QTest::newRow("lineediteventRow2")<<100.0  <<0.0   <<" suffix2"  <<2  <<0  <<"***"  <<true <<keylist2<<"0.00 suffix2";
    QTest::newRow("lineediteventRow3")<<100.0  <<0.0   <<" suffix3"  <<2  <<0  <<"***"  <<true <<keylist3<<"specialvaluetext+suffix";
    QTest::newRow("lineediteventRow4")<<100.0  <<0.0   <<" suffix4"  <<2  <<0  <<"***"  <<true <<keylist4<<"specialvaluetext+suffix";
    QTest::newRow("lineediteventRow5")<<100.0  <<0.0   <<" suffix5"  <<2  <<0  <<"***"  <<true <<keylist5<<"specialvaluetext+suffix";
    QTest::newRow("lineediteventRow6")<<100.0  <<0.0   <<" suffix6"  <<2  <<0  <<"***"  <<true <<keylist6<<"prelineedittext";
    QTest::newRow("lineediteventRow7")<<100.0  <<0.0   <<" suffix7"  <<5  <<0  <<"***"  <<true <<keylist7<<"0.00004 suffix7";
    QTest::newRow("lineediteventRow8")<<100.0  <<0.0   <<" suffix8"  <<5  <<0  <<"***"  <<true <<keylist8<<"0.00000 suffix8";
    QTest::newRow("lineediteventRow9")<<100.0  <<0.0   <<" suffix9"  <<5  <<-1 <<"***"  <<true <<keylist8<<"0.00000 suffix9";


    //    QTest::newRow("lineediteventRow1")<<100.0  <<0.0   <<" suffix1"  <<2  <<0  <<"***"  <<false<<keylist0<<"xxx";
    //    QTest::newRow("lineediteventRow3")<<100.0  <<0.0   <<" suffix3"  <<2  <<0  <<"***"  <<false<<keylist1<<"xxx";
    //    QTest::newRow("lineediteventRow5")<<100.0  <<0.0   <<" suffix5"  <<2  <<0  <<"***"  <<false<<keylist2<<"xxx";
    //    QTest::newRow("lineediteventRow7")<<100.0  <<0.0   <<" suffix7"  <<2  <<0  <<"***"  <<false<<keylist3<<"xxx";
    //    QTest::newRow("lineediteventRow9")<<100.0  <<0.0   <<" suffix9"  <<2  <<0  <<"***"  <<false<<keylist4<<"xxx";
    //    QTest::newRow("lineediteventRowb")<<100.0  <<0.0   <<" suffixb"  <<2  <<0  <<"***"  <<false<<keylist5<<"xxx";
#endif
    QTest::newRow("lineediteventRowd")<<100.0  <<0.0   <<" suffixd"  <<2  <<0  <<"***"  <<false<<keylist6<<"abcdefg"; //onlynumeric的值为false的处理有误

}
void TestICCustomWidgets::testICSlidingWidget()
{
    //    QTest::qWait(1000000);

    ICSlidingWidget widget;
    QLabel widget1("hellow");
    QLabel widget2("Worldd!");
    widget.AddWidget(&widget1);
    widget.AddWidget(&widget1);
    widget.AddWidget(&widget2);
    QCOMPARE(widget.Count(), 2);
    QCOMPARE(widget1.parentWidget(), &widget);
    QCOMPARE(widget2.parentWidget(), &widget);
    QCOMPARE(widget1.x(), widget.x());
    widget.show();
    //    QTest::qWaitForWindowShown(&widget);
    widget.SetCurrentWidget(&widget1, ICSlidingWidget::SlidingWay_SlidingRight);
    QCOMPARE(widget.CurrentWidget(), &widget1);
    QCOMPARE(widget1.x(), widget.x());

    widget.SetCurrentWidget(&widget2, ICSlidingWidget::SlidingWay_None);
    QCOMPARE(widget.CurrentWidget(), &widget2);
    QCOMPARE(widget1.x(), widget.x());
    QCOMPARE(widget2.x(), widget.x());


    QCOMPARE(widget.Widget(0), &widget1);

}

void TestICCustomWidgets::testICUnitLabel()
{
    QFETCH(double,doubleInput);
    QFETCH(int,setDecimalsInt);
    QFETCH(QString,suffix);
    QFETCH(QString, stringResult);

    emit unitlabelSignal(suffix);
    unitlabel->SetValue(doubleInput,setDecimalsInt);

    QCOMPARE(unitlabel->text(),stringResult);
}

void TestICCustomWidgets::testICUnitLabel_data()
{
    QTest::addColumn<double>("doubleInput");    //1
    QTest::addColumn<int>("setDecimalsInt");    //2
    QTest::addColumn<QString>("suffix");        //3
    QTest::addColumn<QString>("stringResult");  //4

    /********************************1********2*******3***************4***************/
    QTest::newRow("unitlabelRow0")<<0.0     <<0     <<""            <<QString("0 ");
    QTest::newRow("unitlabelRow1")<<10.0    <<1     <<"/suffix1"    <<"1.0 /suffix1";
    QTest::newRow("unitlabelRow2")<<-1.0    <<3     <<"/suffix2"    <<"-0.100 /suffix2";
    QTest::newRow("unitlabelRow3")<<-1.0    <<3     <<"/suffix3"    <<"-0.100 /suffix3";  //当连续的给出两次一样的doubleInput，并且两个suffix不同时，将会出错。
    QTest::newRow("unitlabelRow4")<<-2.0    <<-1    <<"/suffix4"    <<"-2 /suffix4";      //没有限制setDecimalsInt小于0时的处理
    //    QTest::newRow("unitlabelRow5")<<-5.0    <<-2    <<"/suffix5"    <<"-0.100 /suffix5";

}

void TestICCustomWidgets::testICTwoStatusLabel()
{
}

void TestICCustomWidgets::testICTwoStatusLabel_data()
{
}

void TestICCustomWidgets::testICTableView_SelectedRowsIndex()
{
}

void TestICCustomWidgets::testICTableView_SelectedRowsIndex_data()
{
}

void TestICCustomWidgets::testICCheckBox()
{
    QFETCH(ICCheckBox*,checkbox);
    QFETCH(int,inputcheckboxstate);
    QFETCH(int,inputcheckboxkey);
    checkboxwidget->show();
    QTest::qWaitForWindowShown(checkboxwidget);
    Qt::CheckState state = (Qt::CheckState)inputcheckboxstate;
    Qt::Key key = (Qt::Key)inputcheckboxkey;
    QTest::qWait(1000);   //如果没有这一句，下面的hasFocus()将返回false.???
    checkbox->setFocus();
    QCOMPARE(checkbox->hasFocus(),true);
    checkbox->setCheckState(state); //向对象checkbox_2赋一个给定的值：state
    QTest::keyPress(checkbox,key);  //模似按键按下事件，键值为：key
    //    if(key == Qt::Key_Tab || key == Qt::Key_Backtab)
    //        QCOMPARE(checkbox->hasFocus(),false);
    if(key == Qt::Key_Return)
        QCOMPARE(checkbox->checkState() == state,false);
    else
        QCOMPARE(checkbox->checkState() == state,true);
}

void TestICCustomWidgets::testICCheckBox_data()
{
    QTest::addColumn<ICCheckBox*>("checkbox");
    QTest::addColumn<int>("inputcheckboxstate");
    QTest::addColumn<int>("inputcheckboxkey");


    QTest::newRow("checkboxRow0")<<checkboxwidget->checkbox_1<<(int)Qt::Unchecked         <<(int)Qt::Key_Return;
#ifdef CHECKBOX
    QTest::newRow("checkboxRow1")<<checkboxwidget->checkbox_1<<(int)Qt::Unchecked         <<(int)Qt::Key_A;
    QTest::newRow("checkboxRow2")<<checkboxwidget->checkbox_1<<(int)Qt::Unchecked         <<(int)Qt::Key_Tab;
    QTest::newRow("checkboxRow3")<<checkboxwidget->checkbox_2<<(int)Qt::Checked           <<(int)Qt::Key_Return;
    QTest::newRow("checkboxRow4")<<checkboxwidget->checkbox_2<<(int)Qt::Checked           <<(int)Qt::Key_0;
    QTest::newRow("checkboxRow5")<<checkboxwidget->checkbox_2<<(int)Qt::Checked           <<(int)Qt::Key_Tab;
    QTest::newRow("checkboxRow6")<<checkboxwidget->checkbox_3<<(int)Qt::PartiallyChecked  <<(int)Qt::Key_Return;
    QTest::newRow("checkboxRow7")<<checkboxwidget->checkbox_3<<(int)Qt::PartiallyChecked  <<(int)Qt::Key_Down;
    QTest::newRow("checkboxRow8")<<checkboxwidget->checkbox_3<<(int)Qt::PartiallyChecked  <<(int)Qt::Key_Backtab;
#endif
}

void TestICCustomWidgets::testICClockLabel()
{
    clocklabel->show();
    QCOMPARE(clocklabel->timer_status(),true);
    clocklabel->hide();
    QCOMPARE(clocklabel->timer_status(),false);
}

void TestICCustomWidgets::testICComboBox()
{
    QStringList items;
    items <<"a"<<"b"<<"c"<<"d"<<"e"<<"f";
    combobox->combobox_1->addItems(items);
    combobox->combobox_2->addItems(items);
    combobox->combobox_3->addItems(items);

    combobox->show();
    QTest::qWaitForWindowShown(combobox);
    QTest::keyClick(combobox->combobox_1,Qt::Key_Return);
    QTest::keyClick(NULL,Qt::Key_Down);
    QTest::keyClick(NULL,Qt::Key_Tab);
    QTest::keyClick(NULL,Qt::Key_Return);

    //#ifdef XXXX
    //    QTestEventList events;
    //    events.addKeyClick(Qt::Key_Tab);
    //    events.addKeyClick(Qt::Key_Return);
    //    events.addKeyClick(Qt::Key_Down);
    //    events.simulate(combobox);
    //    while(1)
    //    {
    //    qDebug("1Key_Return");
    //    QTest::keyPress(combobox->combobox_1,Qt::Key_Return);
    //    QTest::qWait(2000);
    //    qDebug("2Key_Return");
    //    QTest::keyPress(NULL,Qt::Key_Return);
    //    QTest::qWait(2000);
    //    qDebug("3Key_Tab");
    //    QTest::keyPress(NULL,Qt::Key_Tab);
    //    QTest::qWait(2000);
    //    qDebug("4Key_Return");
    //    QTest::keyPress(NULL,Qt::Key_Return);
    //    QTest::qWait(2000);
    //    qDebug("5Key_Down");
    //    QTest::keyPress(NULL,Qt::Key_Down);
    //    QTest::qWait(2000);
    //    qDebug("6Key_Return");
    //    QTest::keyPress(NULL,Qt::Key_Return);
    //    QTest::qWait(2000);
    //    }
    //    combobox->combobox_1->hidePopup();
    //    qDebug()<<combobox->combobox_1->hasFocus();
    //    combobox->combobox_1->is
    //    QString combobox_1_CurrentText = combobox->combobox_1->currentText();
    //    events.simulate(combobox->combobox_3);
    //    QTest::qWait(1000000);
    //    QCOMPARE(combobox->currentText() == comboboxCurrentText,false);
    //    QTest::qWait(1000);
    ////    combobox->setFocus();
    //    qDebug()<<combobox->hasFocus();
    //    QString comboboxCurrentText = combobox->currentText();
    //    while(1)
    //    {
    //        combobox->setFocus();
    //        qDebug()<<combobox->hasFocus();
    //        qDebug("1");
    //        QTest::keyPress(combobox,Qt::Key_Return);
    //        qDebug()<<combobox->hasFocus();
    //        //    QTest::qWait(1000);
    //        //    qDebug()<<combobox->hasFocus();
    //        QTest::qWait(1000);
    //        QTest::keyPress(NULL,Qt::Key_Down);
    //        qDebug("2");
    //        QTest::qWait(1000);
    //        QTest::keyPress(NULL,Qt::Key_Return);
    //        qDebug("3");
    //        qDebug()<<combobox->hasFocus();
    //        QTest::qWait(1000);
    //        qDebug("4");
    //        QTest::keyPress(NULL,Qt::Key_Return);
    //        qDebug("5");
    //        QTest::qWait(1000);
    //        QTest::keyPress(NULL,Qt::Key_Return);
    //        qDebug("6");
    //        QTest::qWait(1000);
    //    }
    //    QCOMPARE(combobox->currentText() == comboboxCurrentText,false);
    //    QTest::qWait(1000000);
    //#endif
}

void TestICCustomWidgets::testICDoubleSpinBox()
{
    QFETCH(QString,inputtext);
    QFETCH(QString,expect);
    doublespinbox->OnLineEditTextChanged(inputtext);
    QCOMPARE(doublespinbox->text() == expect, true);
}

void TestICCustomWidgets::testICDoubleSpinBox_data()
{
    QTest::addColumn<QString>("inputtext");
    QTest::addColumn<QString>("expect");
    QTest::newRow("doublespinboRow0")<<"30"<<"1.50";
}

void TestICCustomWidgets::testICFrameButton()
{
}

void TestICCustomWidgets::testICGroupBox()
{
    QFETCH(int,inputkey);
    QFETCH(bool,prechecked);
    Qt::Key key = (Qt::Key)inputkey;
    groupbox->setCheckable(prechecked);
    QTest::keyPress(groupbox,key);
    if(prechecked && key == Qt::Key_Return)
        QCOMPARE(groupbox->isChecked(),!prechecked);
    else
        QCOMPARE(groupbox->isChecked(),prechecked);
}

void TestICCustomWidgets::testICGroupBox_data()
{
    QTest::addColumn<int>("inputkey");
    QTest::addColumn<bool>("prechecked");
    QTest::newRow("groupboxRow0")<<(int)Qt::Key_Return<<true;
    QTest::newRow("groupboxRow1")<<(int)Qt::Key_Return<<false;
    QTest::newRow("groupboxRow2")<<(int)Qt::Key_0<<true;
    QTest::newRow("groupboxRow3")<<(int)Qt::Key_Tab<<false;
}

void TestICCustomWidgets::testICSpinBox()
{
    QTestEventList events;
    QFETCH(ICSpinBox *,inputobject);
    QFETCH(QList<Qt::Key>,inputkey);
    QFETCH(int,objectminimum);
    QFETCH(int,objectmaximum);
    QFETCH(QString,expect);

    spinbox->show();
    QTest::qWaitForWindowShown(spinbox);
    inputobject->setMinimum(objectminimum);
    inputobject->setMaximum(objectmaximum);
    for(int i=0;i<inputkey.size();i++)
        events.addKeyPress(inputkey.at(i));
    events.addKeyPress(Qt::Key_Return);
    QTest::qWait(100);
    inputobject->setFocus();
    events.simulate(inputobject);
    QCOMPARE(inputobject->text(),expect);
}

void TestICCustomWidgets::testICSpinBox_data()
{
    QTest::addColumn<ICSpinBox *>("inputobject");
    QTest::addColumn<QList<Qt::Key> >("inputkey");
    QTest::addColumn<int>("objectminimum");
    QTest::addColumn<int>("objectmaximum");
    QTest::addColumn<QString>("expect");

    QList<Qt::Key> keylist0 = QList<Qt::Key>()<< Qt::Key_0;
    QList<Qt::Key> keylist1 = QList<Qt::Key>()<< Qt::Key_9 <<Qt::Key_1;
    QList<Qt::Key> keylist2 = QList<Qt::Key>()<< Qt::Key_2 <<Qt::Key_2<<Qt::Key_1;
    QList<Qt::Key> keylist3 = QList<Qt::Key>()<< Qt::Key_1 <<Qt::Key_1<<Qt::Key_1<<Qt::Key_3;
    QList<Qt::Key> keylist4 = QList<Qt::Key>()<< Qt::Key_1 <<Qt::Key_2<<Qt::Key_1<<Qt::Key_3;
    QList<Qt::Key> keylist5 = QList<Qt::Key>()<< Qt::Key_2 <<Qt::Key_0<<Qt::Key_1<<Qt::Key_Return;
    QList<Qt::Key> keylist6 = QList<Qt::Key>()<< Qt::Key_A <<Qt::Key_B<<Qt::Key_C<<Qt::Key_2;
    QList<Qt::Key> keylist7 = QList<Qt::Key>()<< Qt::Key_3 <<Qt::Key_2<<Qt::Key_A<<Qt::Key_Return<<Qt::Key_3;


    /*****************************inputobject*******inputkey***objectminimum**objectmaximum***expect**/
    QTest::newRow("spinboxRow0")<<spinbox->spinbox_1 <<keylist0    <<0             <<50         <<"0";
#ifdef SPINBOX
    QTest::newRow("spinboxRow1")<<spinbox->spinbox_2 <<keylist1    <<0             <<99         <<"91";
    QTest::newRow("spinboxRow2")<<spinbox->spinbox_3 <<keylist2    <<0             <<100        <<"22";
    QTest::newRow("spinboxRow3")<<spinbox->spinbox_1 <<keylist3    <<0             <<199        <<"111";
    QTest::newRow("spinboxRow4")<<spinbox->spinbox_2 <<keylist4    <<0             <<200        <<"121";
    QTest::newRow("spinboxRow5")<<spinbox->spinbox_3 <<keylist5    <<0             <<299        <<"201";
    QTest::newRow("spinboxRow6")<<spinbox->spinbox_1 <<keylist6    <<0             <<399        <<"2";
    QTest::newRow("spinboxRow7")<<spinbox->spinbox_2 <<keylist7    <<0             <<499        <<"3";
#endif
}

void TestICCustomWidgets::testICRecordTableModel()
{
    QFETCH(int,location);
    QFETCH(Qt::Orientation,orientation);
    QFETCH(QString,headername);
    QFETCH(bool,isexpectheader);

    QStringList column0stringlist,column1stringlist;
    isreceiverecordtablemodelsignal = false;
    QCOMPARE(recordtablemodel->setHeaderData(location,orientation,headername),isexpectheader);
    if(isexpectheader == true)
    {
        QCOMPARE(isreceiverecordtablemodelsignal,true);
        QCOMPARE(recordtablemodel->headerData(location,orientation).toString(),headername);
    }


    int numrows = recordtablemodel->rowCount();
    int numcolumn = recordtablemodel->columnCount();
    QCOMPARE(numcolumn,2);
    for(int column = 0;column < numcolumn;++column)
    {
        for (int row = 0;row < numrows;++row)
        {
            QModelIndex index = recordtablemodel->index(row,column);
            QString text = recordtablemodel->data(index).toString();
            if(column == 0)
                column0stringlist.append(text);
            else
                column1stringlist.append(text);
        }
    }
    //    for(int name = 0;name<column0stringlist.size();++name)
    //        qDebug()<<column0stringlist.at(name);
    //    for(int property = 0;property<column1stringlist.size();++property)
    //        qDebug()<<column1stringlist.at(property);

}

void TestICCustomWidgets::testICRecordTableModel_data()
{
    QTest::addColumn<int>("location");
    QTest::addColumn<Qt::Orientation>("orientation");
    QTest::addColumn<QString>("headername");
    QTest::addColumn<bool>("isexpectheader");

    QTest::newRow("recordtablemodelRow0")<<0<<Qt::Horizontal<<""<<true;
    QTest::newRow("recordtablemodelRow1")<<1<<Qt::Horizontal<<"name"<<true;
    QTest::newRow("recordtablemodelRow2")<<2<<Qt::Horizontal<<"name"<<false;
    //    QTest::newRow("recordtablemodelRow3")<<-1<<Qt::Horizontal<<"name"<<false;

    QTest::newRow("recordtablemodelRow4")<<0    <<Qt::Vertical<<"name"<<false;
    QTest::newRow("recordtablemodelRow5")<<1    <<Qt::Vertical<<"name"<<false;
    QTest::newRow("recordtablemodelRow6")<<-1   <<Qt::Vertical<<"name"<<false;
    QTest::newRow("recordtablemodelRow7")<<1000 <<Qt::Vertical<<"name"<<false;
}

void TestICCustomWidgets::recordtablemodelSlot()
{
    isreceiverecordtablemodelsignal = true;
}

void TestICCustomWidgets::testICUpdatePackModel()
{
    QFETCH(int,location);
    QFETCH(Qt::Orientation,orientation);
    QFETCH(QString,headername);
    QFETCH(bool,isexpectheader);

    QStringList column0stringlist,column1stringlist;
    isupdatepackmodelsignal = false;
    QCOMPARE(updatepackmodel->setHeaderData(location,orientation,headername),isexpectheader);
    if(isexpectheader == true)
    {
        QCOMPARE(isupdatepackmodelsignal,true);
        QCOMPARE(updatepackmodel->headerData(location,orientation).toString(),headername);
    }

    int numrows = updatepackmodel->rowCount();
    int numcolumn = updatepackmodel->columnCount();
    QCOMPARE(numcolumn,2);
    for(int column = 0;column < numcolumn;++column)
    {
        for (int row = 0;row < numrows;++row)
        {
            QModelIndex index = updatepackmodel->index(row,column);
            QString text = updatepackmodel->data(index).toString();
            if(column == 0)
                column0stringlist.append(text);
            else
                column1stringlist.append(text);
        }
    }
    for(int name = 0;name<column0stringlist.size();++name)
        qDebug()<<column0stringlist.at(name);
    for(int property = 0;property<column1stringlist.size();++property)
        qDebug()<<column1stringlist.at(property);
}

void TestICCustomWidgets::testICUpdatePackModel_data()
{
    QTest::addColumn<int>("location");
    QTest::addColumn<Qt::Orientation>("orientation");
    QTest::addColumn<QString>("headername");
    QTest::addColumn<bool>("isexpectheader");

    QTest::newRow("recordtablemodelRow0")<<0<<Qt::Horizontal<<""<<true;
}

void TestICCustomWidgets::updatepackmodelSlot()
{
    isupdatepackmodelsignal = true;
}

void TestICCustomWidgets::testICAlarmTableModel()
{
    QFETCH(int,databaserow);
    QFETCH(int,levelcolumn);
    QFETCH(int,expectvalue);

    alarmtablemodel->SetLevelColumn(levelcolumn);
    QCOMPARE(alarmtablemodel->LevelColumn(),levelcolumn);
    QModelIndex index = alarmtablemodel->index(databaserow,0);
    QCOMPARE(alarmtablemodel->IndexLevel(index),expectvalue);
}

void TestICCustomWidgets::testICAlarmTableModel_data()
{
    QTest::addColumn<int>("databaserow");
    QTest::addColumn<int>("levelcolumn");
    QTest::addColumn<int>("expectvalue");

    QTest::newRow("alarmtablemodelRow0")<<0 <<-1 <<-1;
    QTest::newRow("alarmtablemodelRow1")<<1 <<0  <<1898282;
    QTest::newRow("alarmtablemodelRow2")<<2 <<1  <<207;
    QTest::newRow("alarmtablemodelRow3")<<3 <<2  <<4;

    /*当行和列的值溢出时是不是应该返回-1*/
    //    QTest::newRow("alarmtablemodelRow4")<<-4  <<3  <<-1;
    //    QTest::newRow("alarmtablemodelRow4")<<-4  <<10 <<-1;
    //    QTest::newRow("alarmtablemodelRow5")<<122 <<0  <<-1;
    //    QTest::newRow("alarmtablemodelRow5")<<122 <<10 <<-1;
}

void TestICCustomWidgets::testICTimeScalePlot_Construct()
{
    ICTimeScalePlot plot;
    QCOMPARE(plot.TimeInterval(), 1000);
    QCOMPARE(plot.XAxisTitle().text(), QString());
    QCOMPARE(plot.YAxisTitle().text(), QString());
    //    QCOMPARE(plot.CurvesCount(), 0);
}

void TestICCustomWidgets::testICTimeScalePlot_SetXAxisTitle()
{
    ICTimeScalePlot plot;
    QString xLabel("X Axis");
    plot.SetXAxisTitle(xLabel);
    QCOMPARE(plot.XAxisTitle().text(), xLabel);
    xLabel = "XX Axis";
    plot.SetXAxisTitle(xLabel);
    QCOMPARE(plot.XAxisTitle().text(), xLabel);
}

void TestICCustomWidgets::testICTimeScalePlot_SetYAxisTitle()
{
    ICTimeScalePlot plot;
    QString yLabel("Y Axis");
    plot.SetYAxisTitle(yLabel);
    QCOMPARE(plot.YAxisTitle().text(), yLabel);
    yLabel = "YY Axis";
    plot.SetYAxisTitle(yLabel);
    QCOMPARE(plot.YAxisTitle().text(), yLabel);
}

void TestICCustomWidgets::testICTimeScalePlot_SetYAxisScale()
{
    ICTimeScalePlot plot;
    plot.SetYAxisScale(0, 1000, 1);
}

void TestICCustomWidgets::testICSqlTableView_Construct()
{
    ICSqlTableView view;
    QCOMPARE(view.SqlTableModel() == NULL, true);
    QCOMPARE(view.TableName(), QString());
}

void TestICCustomWidgets::testICSqlTableView_SetSqlTableModel()
{
    ICSqlTableView view;
    view.SetSqlTableModel(NULL);
    QCOMPARE(view.SqlTableModel() == NULL, true);
    QSqlTableModel model, model1;
    view.SetSqlTableModel(&model);
    QCOMPARE(view.SqlTableModel(), &model);

    view.SetSqlTableModel(&model1);
    QCOMPARE(view.SqlTableModel(), &model1);

}

void TestICCustomWidgets::testICSqlTableView_SetTableName()
{
    ICSqlTableView view;
    view.SetTableName("");
    QCOMPARE(view.TableName(), QString());
    view.SetTableName("TestTable");
    QCOMPARE(view.TableName(), QString("TestTable"));
    QCOMPARE(view.SqlTableModel() != NULL, true);

    QSqlTableModel model;
    view.SetSqlTableModel(&model);
    QCOMPARE(view.SqlTableModel(), &model);
    model.setTable("TestTable1");
    QCOMPARE(view.TableName(), QString("TestTable1"));
}

void TestICCustomWidgets::testICSqlTableView_InsertRecord()
{
//    ICSqlTableView view;
//    view.SetTableName("testtable", testDB);
//    QCOMPARE(view.RecordCount(), 0);
//    QSqlRecord record = view.Record();
//    QCOMPARE(record.value(0).toInt(), 0);
//    QCOMPARE(record.value(1).toString(), QString(""));
////    record.setValue(0, 1);
//    record.setValue(1, "Test");
//    QCOMPARE(view.InsertRecord(0, record), true);
//    QCOMPARE(view.RecordCount(), 1);
//    QSqlRecord inserted = view.Record(0);
////    QCOMPARE(record, inserted);
//    QCOMPARE(inserted.value(0).toInt(), 1);
//    QCOMPARE(inserted.value(1), record.value(1));

//    QCOMPARE(view.InsertRecord(0, record), true);
//    QCOMPARE(view.RecordCount(), 2);
//    QCOMPARE(view.InsertRecord(10, record), false);
//    QCOMPARE(view.RecordCount(), 2);
//    inserted = view.Record(1);
//    QCOMPARE(inserted.value(0).toInt(), 2);
//    QCOMPARE(inserted.value(1), record.value(1));
//    QCOMPARE(view.InsertRecord(-1, record), true);
//    QCOMPARE(view.RecordCount(), 3);

}

void TestICCustomWidgets::testICSqlTableView_ClearRecord()
{
//    ICSqlTableView view;
//    view.SetTableName("testtable", testDB);
//    QCOMPARE(view.ClearRecords(), true);
//    QSqlRecord record = view.Record();
//    record.setValue(1, "Test");
//    QCOMPARE(view.InsertRecord(0, record), true);
//    QCOMPARE(view.InsertRecord(0, record), true);
//    QCOMPARE(view.InsertRecord(0, record), true);
//    QCOMPARE(view.ClearRecords(), true);
//    QCOMPARE(view.RecordCount(), 0);
}

void TestICCustomWidgets::testICSqlTableView_SetSort()
{
//    ICSqlTableView view;
//    view.SetTableName("testtable", testDB);
//    QCOMPARE(view.ClearRecords(), true);
//    QSqlRecord record = view.Record();
//    record.setValue(1, "Test");
//    QCOMPARE(view.InsertRecord(0, record), true);
//    QCOMPARE(view.InsertRecord(0, record), true);
//    QCOMPARE(view.InsertRecord(0, record), true);
//    record = view.Record(0);
//    QCOMPARE(record.value(0).toInt(), 1);
//    view.SetSort(0, Qt::DescendingOrder);
//    record = view.Record(0);
//    QCOMPARE(record.value(0).toInt(), 3);

}

void TestICCustomWidgets::testIcSqlTableView_SetHeaderData()
{
//    ICSqlTableView view;
//    view.SetTableName("testtable", testDB);
//    QCOMPARE(view.SetHeaderData(0, Qt::Horizontal, tr("id")), true);
//    QCOMPARE(view.HeaderData(0, Qt::Horizontal).toString(), QString(tr("id")));
}

QTEST_MAIN(TestICCustomWidgets);

#include "tst_testiccustomwidgets.moc"

