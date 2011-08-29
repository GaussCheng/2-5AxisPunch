#ifndef ICLINEEDITWITHVIRTUALKEYBOARD_H
#define ICLINEEDITWITHVIRTUALKEYBOARD_H

#include <QLineEdit>

class QWidget;
class VirtualKeyboardDialog;

class ICLineEditWithVirtualKeyboard : public QLineEdit
{
    Q_OBJECT
public:
    ICLineEditWithVirtualKeyboard(QWidget * parent = 0);
    ~ICLineEditWithVirtualKeyboard();

protected:
    void mouseReleaseEvent(QMouseEvent *);

private:
    VirtualKeyboardDialog * virtualKeyboardDialog_;

private slots:
    void SetCurrentText(const QString &currentText);
};

#endif // LINEEDITWITHVIRTUALKEYBOARD_H
