#ifndef ICPNEUMATICACTIONPAGE_H
#define ICPNEUMATICACTIONPAGE_H

#include <QMap>
#include <QPair>
#include <QSignalMapper>
#include <QPixmap>
#include "icinstructioneditorbase.h"
#include "icsystemconfig.h"

namespace Ui {
    class ICPneumaticActionPage;
}

class QAbstractButton;
class QLabel;
class ICPeripheryParameterEditor;
class ICPneumaticActionPage : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICPneumaticActionPage(QWidget *parent = 0);
    ~ICPneumaticActionPage();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);
    QList<ICMoldItem> CreateCommandImpl() const;
    void SyncStatusImpl(const QList<ICMoldItem> &items) {Q_UNUSED(items)}

private Q_SLOTS:
    void CommandButtonClicked(QWidget* widget);

private:
    Ui::ICPneumaticActionPage *ui;
    QPixmap onPixmap_;
    QPixmap offPixmap_;
    QMap<int, ICUserActionInfo> rowToInfoMap_;
};

#endif // ICPNEUMATICACTIONPAGE_H
