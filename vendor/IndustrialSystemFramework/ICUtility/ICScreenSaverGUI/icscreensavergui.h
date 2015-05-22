#ifndef ICSCREENSAVERGUI_H
#define ICSCREENSAVERGUI_H

#include <QWidget>
class QLabel;
class QPushButton;


class ICScreenSaverGUI : public QWidget
{
    Q_OBJECT

public:
    explicit ICScreenSaverGUI(QWidget *parent = 0);
    ~ICScreenSaverGUI();

private:
    QLabel *backgroundLabel_;
    QPushButton *unlockButton_;
};

#endif // ICSCREENSAVERGUI_H
