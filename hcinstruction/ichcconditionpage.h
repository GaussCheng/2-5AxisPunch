#ifndef ICHCCONDITIONPAGE_H
#define ICHCCONDITIONPAGE_H

#include <QFrame>
#include "icinstructioneditorbase.h"

namespace Ui {
    class ICHCConditionPage;
}

class ICHCConditionPage : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICHCConditionPage(QWidget *parent = 0);
    ~ICHCConditionPage();

protected:
    void changeEvent(QEvent *);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;

private:
    Ui::ICHCConditionPage *ui;

private slots:
};

#endif // ICHCCONDITIONPAGE_H
