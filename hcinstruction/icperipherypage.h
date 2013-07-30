#ifndef ICPERIPHERYPAGE_H
#define ICPERIPHERYPAGE_H

#include <QMap>
#include <QPair>
#include <QSignalMapper>
#include <QPixmap>
#include "icinstructioneditorbase.h"
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
    QVector<QAbstractButton*> settingButtons_;
    QVector<ICPeripheryParameterEditor*> editorVector_;
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

#endif // ICPERIPHERYPAGE_H
