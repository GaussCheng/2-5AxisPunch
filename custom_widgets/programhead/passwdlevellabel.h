#ifndef PASSWDLEVELLABEL_H
#define PASSWDLEVELLABEL_H

#include <QLabel>
#include "passworddialog.h"

class PasswdLevelLabel : public QLabel
{
    Q_OBJECT

public:

    PasswdLevelLabel(QWidget * parent = 0);
    ~PasswdLevelLabel();

    int CurrentLevel() const { return currentLevel_;}

Q_SIGNALS:
    void Levelchenged(int level);

public slots:
    void PasswdLevelChenged(int level);
protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void changeEvent(QEvent *e);

private slots:

private:
    PasswordDialog* passwordDialog_;
    int currentLevel_;
};

#endif // PASSWDLEVELLABEL_H
