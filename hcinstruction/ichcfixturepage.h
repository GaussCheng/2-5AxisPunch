#ifndef ICHCFIXTUREPAGE_H
#define ICHCFIXTUREPAGE_H

#include <QFrame>
#include <QMap>
#include <QPair>
#include <QSignalMapper>
#include <QPixmap>
#include "icinstructioneditorbase.h"
#include "iclineeditwithvirtualnumerickeypad.h"

class QAbstractButton;
class QLabel;

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

private:
    Ui::ICHCFixturePage *ui;
    QVector<QAbstractButton*> settingButtons_;
    QVector<ICLineEditWithVirtualNumericKeypad*> delayEdits_;
    QMap<QAbstractButton*, uint> buttonToClip_;
    QMap<uint, uint> onClipToOffClip_;
    QMap<uint, uint> offClipToOnClip_;
    QStringList ioNames_;
    QMap<QAbstractButton*, QPair<int, int> > commandKeyMap_;
    QSignalMapper buttonSignalMapper_;
    QPixmap onPixmap_;
    QPixmap offPixmap_;
};

#endif // ICHCFIXTUREPAGE_H
