#ifndef ICPNEUMATICACTIONPAGE_H
#define ICPNEUMATICACTIONPAGE_H

#include <QMap>
#include <QPair>
#include <QSignalMapper>
#include <QPixmap>
#include "icinstructioneditorbase.h"

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

#endif // ICPNEUMATICACTIONPAGE_H
