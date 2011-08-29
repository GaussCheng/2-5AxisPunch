#ifndef ICMACHINECONFIGPAGE_H
#define ICMACHINECONFIGPAGE_H

#include <QWidget>

class ICLineEditWrapper;

namespace Ui {
    class ICMachineConfigPage;
}

class ICMachineConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit ICMachineConfigPage(QWidget *parent = 0);
    ~ICMachineConfigPage();

protected:
    void changeEvent(QEvent *e);
    void hideEvent(QHideEvent *e);

private:
    Ui::ICMachineConfigPage *ui;
    QList<ICLineEditWrapper*> wrappers_;
};

#endif // ICMACHINECONFIGPAGE_H
