#ifndef ICHCTIMEFRAME_H
#define ICHCTIMEFRAME_H

#include <QFrame>
#include <QList>

class QLineEdit;
class ICLineEditWrapper;

namespace Ui {
    class ICHCTimeFrame;
}

class ICHCTimeFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCTimeFrame(QWidget *parent = 0);
    ~ICHCTimeFrame();

protected:
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);
private:
    void InitInterFace();
    void BindingParam_(QLineEdit* edit, int addr);

private:
    Ui::ICHCTimeFrame *ui;
    QList<ICLineEditWrapper*> wrappers_;
};

#endif // ICHCTIMEFRAME_H
