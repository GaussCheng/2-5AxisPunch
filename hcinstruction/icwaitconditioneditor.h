#ifndef ICWAITCONDITIONEDITOR_H
#define ICWAITCONDITIONEDITOR_H

#include "icinstructioneditorbase.h"

namespace Ui {
    class ICWaitConditionEditor;
}

class ICWaitConditionEditor : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICWaitConditionEditor(QWidget *parent = 0);
    ~ICWaitConditionEditor();

protected:
    void changeEvent(QEvent *e);
    QList<ICMoldItem> CreateCommandImpl() const;
    void SyncStatusImpl(const QList<ICMoldItem> &items) {Q_UNUSED(items);}

private:
    Ui::ICWaitConditionEditor *ui;
};

#endif // ICWAITCONDITIONEDITOR_H
