#ifndef ICHCOTHERPAGE_H
#define ICHCOTHERPAGE_H

#include "icinstructioneditorbase.h"

namespace Ui {
class ICHCOtherPage;
}

class ICHCOtherPage : public ICInstructionEditorBase
{
    Q_OBJECT
    
public:
    explicit ICHCOtherPage(QWidget *parent = 0);
    ~ICHCOtherPage();
    
protected:
//    void changeEvent(QEvent *);
//    void showEvent(QShowEvent *e);
//    void hideEvent(QHideEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items){}
    QList<ICMoldItem> CreateCommandImpl() const;

private:
    Ui::ICHCOtherPage *ui;
};

#endif // ICHCOTHERPAGE_H
