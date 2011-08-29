#ifndef ICINSTRUCTIONEDITORBASE_H
#define ICINSTRUCTIONEDITORBASE_H

#include <QFrame>
#include "icmold.h"

namespace Ui {
    class ICInstructionEditorBase;
}

class ICInstructionEditorBase : public QFrame
{
    Q_OBJECT

public:
    explicit ICInstructionEditorBase(QWidget *parent = 0);
    ~ICInstructionEditorBase();
    void SyncStatus(const QList<ICMoldItem> &items) { SyncStatusImpl(items);}

    QList<ICMoldItem> CreateCommand() const {return CreateCommandImpl();}

protected:
    virtual void SyncStatusImpl(const QList<ICMoldItem> &items) = 0;
    virtual QList<ICMoldItem> CreateCommandImpl() const = 0;
private:
    Ui::ICInstructionEditorBase *ui;
};

#endif // ICINSTRUCTIONEDITORBASE_H
