#ifndef ICFLAGSEDITOR_H
#define ICFLAGSEDITOR_H

#include "icinstructioneditorbase.h"

namespace Ui {
    class ICFlagsEditor;
}

class ICFlagsEditor : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICFlagsEditor(QWidget *parent = 0);
    ~ICFlagsEditor();

    ICTopMoldUIItem CreateTopUIItem() const;
protected:
    void changeEvent(QEvent *e);
    QList<ICMoldItem> CreateCommandImpl() const;
    void SyncStatusImpl(const QList<ICMoldItem> &items){Q_UNUSED(items)}

private:
    Ui::ICFlagsEditor *ui;
    ICTopMoldUIItem mainArmInsideGet_;
    ICTopMoldUIItem subArmInsideGet_;
    ICTopMoldUIItem mainArmOutsideGet_;
    ICTopMoldUIItem subArmOUtsideGet_;
//    QVector<ICMoldUIItem::FlagType> flags_;
};

#endif // ICFLAGSEDITOR_H
