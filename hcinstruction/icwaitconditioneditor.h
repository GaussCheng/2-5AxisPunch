#ifndef ICWAITCONDITIONEDITOR_H
#define ICWAITCONDITIONEDITOR_H

#include "icinstructioneditorbase.h"
#include "icsystemconfig.h"
class QTableWidgetItem;

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

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_euInList_itemChanged(QTableWidgetItem *item);

    void on_euOutList_itemChanged(QTableWidgetItem *item);

private:
    Ui::ICWaitConditionEditor *ui;
    QPixmap onPixmap_;
    QPixmap offPixmap_;
//    QMap<int, ICUserIOInfo> xrowToInfoMap_;
//    QMap<int, ICUserIOInfo> yrowToInfoMap_;
};

#endif // ICWAITCONDITIONEDITOR_H
