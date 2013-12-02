#ifndef ICSETTINGSFRAME_H
#define ICSETTINGSFRAME_H

#include <QFrame>

namespace Ui {
class ICSettingsFrame;
}

class ICSettingsFrame : public QFrame
{
    Q_OBJECT
    
public:
    explicit ICSettingsFrame(QWidget *parent = 0);
    ~ICSettingsFrame();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::ICSettingsFrame *ui;
};

#endif // ICSETTINGSFRAME_H
