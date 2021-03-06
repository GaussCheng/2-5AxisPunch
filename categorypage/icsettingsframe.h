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

    void SetToShowAll(bool isshow);
    
protected:
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *e);
    
private slots:
    void on_generateButton_clicked();

    void on_registerButton_clicked();

    void on_fcode_textChanged(const QString &arg1);

private:
    Ui::ICSettingsFrame *ui;
};

#endif // ICSETTINGSFRAME_H
