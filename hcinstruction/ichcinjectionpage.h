#ifndef ICHCINJECTIONPAGE_H
#define ICHCINJECTIONPAGE_H

#include <QFrame>
#include <QMap>
#include <QPair>
#include <QSignalMapper>
#include "icinstructioneditorbase.h"
#include "iclineeditwithvirtualnumerickeypad.h"

class QLabel;
class QAbstractButton;

namespace Ui {
    class ICHCInjectionPage;
}

class ICHCInjectionPage : public ICInstructionEditorBase
{
    Q_OBJECT

public:
    explicit ICHCInjectionPage(QWidget *parent = 0);
    ~ICHCInjectionPage();

protected:
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void SyncStatusImpl(const QList<ICMoldItem> &items);
    QList<ICMoldItem> CreateCommandImpl() const;
private Q_SLOTS:
//    void on_placeOtherEdit_textEdited(QString );
//    void on_placeDelayEdit_textChanged(QString );
//    void on_ejecteOtherEdit_textChanged(QString );
//    void on_ejecteDelayEdit_textChanged(QString );
//    void on_closeMoldOtherEdit_textChanged(QString );
//    void on_closeMoldlDelayEdit_textChanged(QString );
    void StatusRefreshed();
    void CommandButtonClicked(QWidget *widget);

private:
    Ui::ICHCInjectionPage *ui;
    QVector<QAbstractButton*> settingButtons_;
    QVector<ICLineEditWithVirtualNumericKeypad*> delayEdits_;
    QMap<QAbstractButton*, uint> buttonToClip_;
    QMap<QAbstractButton*, uint> buttonToLight_;
    QMap<uint, uint> onClipToOffClip_;
    QMap<uint, uint> offClipToOnClip_;
    QStringList ioNames_;
    QMap<QAbstractButton*, QPair<int, int> > commandKeyMap_;
    QSignalMapper buttonSignalMapper_;
    QPixmap onPixmap_;
    QPixmap offPixmap_;
};

#endif // ICHCINJECTIONPAGE_H
