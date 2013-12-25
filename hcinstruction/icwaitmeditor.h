#ifndef ICWAITMEDITOR_H
#define ICWAITMEDITOR_H

#include "icinstructioneditorbase.h"

namespace Ui {
class ICWaitMEditor;
}

class QTableWidgetItem;

class ICWaitMEditor : public ICInstructionEditorBase
{
    Q_OBJECT
    
public:
    explicit ICWaitMEditor(QWidget *parent = 0);
    ~ICWaitMEditor();
    
protected:
    void changeEvent(QEvent *e);
    QList<ICMoldItem> CreateCommandImpl() const;
    void SyncStatusImpl(const QList<ICMoldItem> &items) {Q_UNUSED(items);}

    
private slots:
    void on_mList_itemChanged(QTableWidgetItem *item);

private:
    Ui::ICWaitMEditor *ui;
};

#endif // ICWAITMEDITOR_H
