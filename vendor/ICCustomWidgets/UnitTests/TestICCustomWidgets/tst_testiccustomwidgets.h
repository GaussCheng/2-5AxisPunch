#ifndef TST_TESTICCUSTOMWIDGETS_H
#define TST_TESTICCUSTOMWIDGETS_H

#include <QWidget>
#include "iccheckbox.h"
#include "iccombobox.h"
#include "icspinbox.h"

class TempWidget : public QWidget
{
//    Q_OBJECT

public:
    explicit TempWidget(QWidget *parent = 0);
    ~TempWidget();

    ICCheckBox *checkbox_1,*checkbox_2,*checkbox_3;

private:
    QGridLayout *gridlayout;
};

class TestComboboxWidget : public QWidget
{
public:
    explicit TestComboboxWidget(QWidget *parent = 0);
    ~TestComboboxWidget();

    ICComboBox *combobox_1,*combobox_2,*combobox_3;

private:
    QGridLayout *gridlayout;
};

class TestSpinboxWidget : public QWidget
{
public:
    explicit TestSpinboxWidget(QWidget *parent = 0);
    ~TestSpinboxWidget();

    ICSpinBox *spinbox_1,*spinbox_2,*spinbox_3;

private:
    QGridLayout *gridlayout;
};

#endif // TST_TESTICCUSTOMWIDGETS_H
