#ifndef ICCOMMENTEDITOR_H
#define ICCOMMENTEDITOR_H

#include "icinstructioneditorbase.h"

namespace Ui {
class ICCommentEditor;
}

class ICCommentEditor : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICCommentEditor(QWidget *parent = 0);
    ~ICCommentEditor();

    QString Comment();

protected:
    void changeEvent(QEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items) {}
    QList<ICMoldItem> CreateCommandImpl() const;

private:
    Ui::ICCommentEditor *ui;
};

#endif // ICCOMMENTEDITOR_H
