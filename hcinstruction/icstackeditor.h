#ifndef ICSTACKEDITOR_H
#define ICSTACKEDITOR_H

#include "icinstructioneditorbase.h"

namespace Ui {
class ICStackEditor;
}

class ICStackEditor : public ICInstructionEditorBase
{
    Q_OBJECT
    
public:
    explicit ICStackEditor(QWidget *parent = 0);
    ~ICStackEditor();
    
protected:
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *);
    void SyncStatusImpl(const QList<ICMoldItem> &items){Q_UNUSED(items)}
    QList<ICMoldItem> CreateCommandImpl() const;

private:
    Ui::ICStackEditor *ui;
};

#endif // ICSTACKEDITOR_H
