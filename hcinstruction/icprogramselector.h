#ifndef ICPROGRAMSELECTOR_H
#define ICPROGRAMSELECTOR_H

#include "icinstructioneditorbase.h"

namespace Ui {
class ICProgramSelector;
}

class ICProgramSelector : public ICInstructionEditorBase
{
    Q_OBJECT
    
public:
    explicit ICProgramSelector(QWidget *parent = 0);
    ~ICProgramSelector();

protected:
    void SyncStatusImpl(const QList<ICMoldItem> &items){}
    QList<ICMoldItem> CreateCommandImpl() const{ return QList<ICMoldItem>();}
    
signals:
    void ProgramChanged(int, QString);
private slots:
    void on_selectButton_clicked();

private:
    Ui::ICProgramSelector *ui;
};

#endif // ICPROGRAMSELECTOR_H
