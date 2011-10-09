#ifndef ICPNEUMATICACTIONPAGE_H
#define ICPNEUMATICACTIONPAGE_H

#include <QWidget>
#include "icinstructioneditorbase.h"

namespace Ui {
    class ICPneumaticActionPage;
}

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
    void SyncStatusImpl(const QList<ICMoldItem> &items) {};

private Q_SLOTS:
    void on_forwardCheckBox_stateChanged(int status);
    void on_backwardCheckBox_stateChanged(int status);
    void on_upCheckBox_stateChanged(int status);
    void on_downCheckBox_stateChanged(int status);

    void on_setButton_clicked();

private:
    Ui::ICPneumaticActionPage *ui;
};

#endif // ICPNEUMATICACTIONPAGE_H
