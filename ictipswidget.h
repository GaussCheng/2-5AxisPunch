#ifndef ICTIPSWIDGET_H
#define ICTIPSWIDGET_H

#include <QDialog>

namespace Ui {
class ICTipsWidget;
}

class ICTipsWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit ICTipsWidget(QWidget *parent = 0);
    explicit ICTipsWidget(const QString& msg, QWidget *parent = 0);
    ~ICTipsWidget();

    void SetMessage(const QString& msg);
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::ICTipsWidget *ui;
};

#endif // ICTIPSWIDGET_H
