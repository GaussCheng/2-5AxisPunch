#ifndef ICCUTPAGE_H
#define ICCUTPAGE_H

#include <QMap>
#include <QSignalMapper>
#include "icinstructioneditorbase.h"
#include "iclineeditwithvirtualnumerickeypad.h"
#include "icsystemconfig.h"

namespace Ui {
    class ICCutPage;
}

class QAbstractButton;

class ICCutPage : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICCutPage(QWidget *parent = 0);
    ~ICCutPage();

protected:
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;

private Q_SLOTS:
    void StatusRefreshed();
    void CommandButtonClicked(QWidget* widget);

private:
    Ui::ICCutPage *ui;
private:
    QPixmap onPixmap_;
    QPixmap offPixmap_;

    QMap<int, ICUserIOInfo> rowToInfoMap_;
};

#endif // ICCUTPAGE_H
