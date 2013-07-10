#ifndef ICAUTORUNREVISE_H
#define ICAUTORUNREVISE_H

#include <QDialog>
#include "icmold.h"
#include <QIntValidator>

namespace Ui {
    class ICAutoRunRevise;
}

class ICAutoRunRevise : public QDialog
{
    Q_OBJECT

public:
    explicit ICAutoRunRevise(QWidget *parent = 0);
    ~ICAutoRunRevise();

    bool ShowModifyItem(const ICMoldItem* item, ICMoldItem* ret,const QString &text);

protected:
    void changeEvent(QEvent *e);

Q_SIGNALS:
    void MoldItemChanged();
private:
    Ui::ICAutoRunRevise *ui;
    ICMoldItem* moldItem_;
    QIntValidator* validator;
    QIntValidator* returnStepValidator;

private slots:
//    void on_buttonBox_accepted();
};

#endif // ICAUTORUNREVISE_H
