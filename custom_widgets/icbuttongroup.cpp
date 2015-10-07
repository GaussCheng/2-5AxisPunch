#include "icbuttongroup.h"

ICButtonGroup::ICButtonGroup(QButtonGroup* bg, QObject *parent) :
    bg_(bg),
    QObject(parent)
{
    connect(bg, SIGNAL(buttonClicked(int)), SLOT(OnIDChanged(int)));
    oldValue_ = bg->checkedId();
    connect(bg, SIGNAL(destroyed()), SLOT(deleteLater()));
}

void ICButtonGroup::OnIDChanged(int id)
{
    emit buttonClicked(id, oldValue_);
    oldValue_ = id;
}
