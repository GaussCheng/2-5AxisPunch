#ifndef ICHCFIXTUREPAGE_H
#define ICHCFIXTUREPAGE_H

#include <QFrame>
#include <QMap>
#include <QPair>
#include <QSignalMapper>
#include <QPixmap>
#include "icinstructioneditorbase.h"
#include "iclineeditwithvirtualnumerickeypad.h"
#include "icsystemconfig.h"

class QAbstractButton;
class QLabel;
class QTableWidgetItem;

namespace Ui {
    class ICHCFixturePage;
}

class ICHCFixturePage : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICHCFixturePage(QWidget *parent = 0);
    ~ICHCFixturePage();

protected:
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;

private Q_SLOTS:
    void StatusRefreshed();
    void CommandButtonClicked(QWidget* widget);

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

private:
    Ui::ICHCFixturePage *ui;
    QPixmap onPixmap_;
    QPixmap offPixmap_;

    QMap<int, ICUserIOInfo> rowToInfoMap_;
};

#endif // ICHCFIXTUREPAGE_H
