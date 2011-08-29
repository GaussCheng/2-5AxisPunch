#include <QComboBox>
#include "ictwoselectioncomboboxwrapper.h"
#include "icmold.h"

ICTwoSelectionComboBoxWrapper::ICTwoSelectionComboBoxWrapper(QComboBox *comboBox, int addr, QObject *parent) :
    QObject(parent),
    wrappedWidget_(comboBox),
    addr_(addr)
{
    wrappedWidget_->setCurrentIndex(qAbs(ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(addr)) - 1));
    connect(wrappedWidget_,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(IndexChanged(int)));
}

void ICTwoSelectionComboBoxWrapper::IndexChanged(int index)
{
    ICMold::CurrentMold()->SetMoldParam(static_cast<ICMold::ICMoldParam>(addr_), qAbs(index - 1));
}

void ICTwoSelectionComboBoxWrapper::UpdateParameter()
{
    disconnect(wrappedWidget_,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(IndexChanged(int)));
    wrappedWidget_->setCurrentIndex(qAbs(ICMold::CurrentMold()->MoldParam(static_cast<ICMold::ICMoldParam>(addr_)) - 1));
    connect(wrappedWidget_,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(IndexChanged(int)));
}
