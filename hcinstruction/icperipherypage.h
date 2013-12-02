#ifndef ICPERIPHERYPAGE_H
#define ICPERIPHERYPAGE_H

#include <QMap>
#include <QPair>
#include <QSignalMapper>
#include <QPixmap>
#include "icinstructioneditorbase.h"
#include "icsystemconfig.h"
//#include "iclineeditwithvirtualnumerickeypad.h"

class QAbstractButton;
class QLabel;
class ICPeripheryParameterEditor;
namespace Ui {
    class ICPeripheryPage;
}

class ICPeripheryPage : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICPeripheryPage(QWidget *parent = 0);
    ~ICPeripheryPage();

protected:
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;

private Q_SLOTS:
    void StatusRefreshed();
    void CommandButtonClicked(QWidget* widget);
///////////////
private:
    Ui::ICPeripheryPage *ui;
    QMap<int, ICUserActionInfo> rowToInfoMap_;
    QPixmap onPixmap_;
    QPixmap offPixmap_;

};

#endif // ICPERIPHERYPAGE_H
