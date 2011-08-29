#ifndef ICAUTORUNREVISE_H
#define ICAUTORUNREVISE_H

#include <QDialog>
#include "icmold.h"

namespace Ui {
    class ICAutoRunRevise;
}

class ICAutoRunRevise : public QDialog
{
    Q_OBJECT

public:
    explicit ICAutoRunRevise(QWidget *parent = 0);
    ~ICAutoRunRevise();

    bool ShowModifyItem(ICMoldItem* item, const QString& text);

Q_SIGNALS:
    void MoldItemChanged();
private:
    Ui::ICAutoRunRevise *ui;
    ICMoldItem* moldItem_;

private slots:
//    void on_buttonBox_accepted();
};

#endif // ICAUTORUNREVISE_H
