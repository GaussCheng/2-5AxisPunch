#ifndef HCMANUALFIXTUREFRAME_H
#define HCMANUALFIXTUREFRAME_H

#include <QFrame>
#include <QList>
#include <QBitArray>

class ICCommandKeyWrapper;

namespace Ui {
    class HCManualFixtureFrame;
}

class HCManualFixtureFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HCManualFixtureFrame(QWidget *parent = 0);
    ~HCManualFixtureFrame();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void StatusRefreshed();

private:
    Ui::HCManualFixtureFrame *ui;
    QList<ICCommandKeyWrapper*> wrappers_;
    QBitArray clips_;
    int timerID_;
};

#endif // HCMANUALFIXTUREFRAME_H
