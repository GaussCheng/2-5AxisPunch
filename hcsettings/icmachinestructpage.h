#ifndef ICMACHINESTRUCTPAGE_H
#define ICMACHINESTRUCTPAGE_H

#include <QWidget>

namespace Ui {
    class ICMachineStructPage;
}

class ICMachineStructPage : public QWidget
{
    Q_OBJECT

public:
    explicit ICMachineStructPage(QWidget *parent = 0);
    ~ICMachineStructPage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ICMachineStructPage *ui;
};

#endif // ICMACHINESTRUCTPAGE_H
