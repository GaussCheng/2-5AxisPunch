#ifndef ICAXISKEYBOARD_H
#define ICAXISKEYBOARD_H

#include <QWidget>

namespace Ui {
class ICAxisKeyboard;
}

class ICAxisKeyboard : public QWidget
{
    Q_OBJECT
    
public:
    typedef ICAxisKeyboard* ICAxisKeyboardPTR ;

    static ICAxisKeyboardPTR Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICAxisKeyboard();

        }
        return instance_;
    }

    void SmartShow(const QWidget* caller);

    ~ICAxisKeyboard();

private slots:
    void OnOneHitKeyPressed();
    void OnConHitKeyPressed();
    void OnConHitKeyRelease();


    void on_close_clicked();

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::ICAxisKeyboard *ui;
    static ICAxisKeyboardPTR instance_;
    explicit ICAxisKeyboard(QWidget *parent = 0);
};

#endif // ICAXISKEYBOARD_H
