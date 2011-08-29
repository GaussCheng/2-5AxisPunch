#ifndef HCMANUALOTHERIOFRAME_H
#define HCMANUALOTHERIOFRAME_H

#include <QWidget>
#include <QList>

class ICCommandKeyWrapper;

namespace Ui {
    class HCManualOtherIOFrame;
}

class HCManualOtherIOFrame : public QWidget
{
    Q_OBJECT

public:
    explicit HCManualOtherIOFrame(QWidget *parent = 0);
    ~HCManualOtherIOFrame();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private Q_SLOTS:
    void StatusRefreshed();

private:
    Ui::HCManualOtherIOFrame *ui;
    QList<ICCommandKeyWrapper*> wrappers_;
};

#endif // HCMANUALOTHERIOFRAME_H
