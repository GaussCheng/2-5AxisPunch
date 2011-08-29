#ifndef BASICSETTINGSFRAME_H
#define BASICSETTINGSFRAME_H

#include <QFrame>
#include <QList>

class ICLineEditWrapper;

namespace Ui {
    class BasicSettingsFrame;
}

class BasicSettingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit BasicSettingsFrame(QWidget *parent = 0);
    ~BasicSettingsFrame();

protected:
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);
private slots:
//    void on_languageComboBox_activated(int index);

private:
    void InitInterface();

private:
    Ui::BasicSettingsFrame *ui;
    QList<ICLineEditWrapper*> wrappers_;

};

#endif // BASICSETTINGSFRAME_H
