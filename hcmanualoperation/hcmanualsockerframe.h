#ifndef HCMANUALSOCKERFRAME_H
#define HCMANUALSOCKERFRAME_H

#include <QFrame>
#include <QList>

class ICCommandKeyWrapper;

namespace Ui {
    class HCManualSockerFrame;
}

class HCManualSockerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HCManualSockerFrame(QWidget *parent = 0);
    ~HCManualSockerFrame();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private Q_SLOTS:
    void StatusRefreshed();

private:
    Ui::HCManualSockerFrame *ui;
    QList<ICCommandKeyWrapper*> wrappers_;
};

#endif // HCMANUALSOCKERFRAME_H
