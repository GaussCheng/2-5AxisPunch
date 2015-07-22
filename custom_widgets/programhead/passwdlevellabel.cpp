#include <QEvent>
#include "passwdlevellabel.h"
#include "icparameterssave.h"
#include "icmodifyframe.h"


PasswdLevelLabel::PasswdLevelLabel(QWidget * parent)
    : QLabel(parent)
{
    passwordDialog_ = new PasswordDialog(this);
    connect(passwordDialog_,
            SIGNAL(LevelChanged(int)),
            this,
            SLOT(PasswdLevelChenged(int)));
    PasswdLevelChenged(ICParametersSave::MachineOperator);
}

PasswdLevelLabel::~PasswdLevelLabel()
{
    delete passwordDialog_;
}

static QString oldStyle;
void PasswdLevelLabel::mousePressEvent(QMouseEvent *ev)
{
    oldStyle = this->styleSheet();



    this->setStyleSheet("padding-left:6px;padding-right: 6px;border-style: inset;border-width: 2px;border-radius: 6px;border-color: gray;background-color: rgba(62, 113, 255, 255);");
}

void PasswdLevelLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    passwordDialog_->show();
    QLabel::mouseReleaseEvent(ev);
    this->setStyleSheet(oldStyle);
}

void PasswdLevelLabel::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        PasswdLevelChenged(currentLevel_);
        break;
    default:
        break;
    }
}

void PasswdLevelLabel::PasswdLevelChenged(int level)
{
       QString oldLevel, newLevel;
        if(currentLevel_ == ICParametersSave::MachineOperator)
            oldLevel = tr("Machine Operator");
        else if(currentLevel_ == ICParametersSave::MachineAdmin)
            oldLevel = tr("Machine Admin");
        else if(currentLevel_ == ICParametersSave::AdvanceAdmin)
            oldLevel = tr("Advance Admin");
        else
            oldLevel = tr("None Level");
        currentLevel_ = level;
        if(level == ICParametersSave::MachineOperator)
        {
            newLevel = tr("Machine Operator");
        }
        else if(level == ICParametersSave::MachineAdmin)
        {
            newLevel = (tr("Machine Admin"));
        }
        else if(level == ICParametersSave::AdvanceAdmin)
        {
            newLevel = (tr("Advance Admin"));
        }

        setText(newLevel);
        ICModifyFrame::Instance()->OnActionTriggered(ICConfigString::kCS_User_Changed,
                                        newLevel,
                                        oldLevel);
        emit Levelchenged(currentLevel_);
}
