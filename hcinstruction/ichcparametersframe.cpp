#include <QMessageBox>
#include "ichcparametersframe.h"
#include "ui_ichcparametersframe.h"

#include "icinstructparam.h"
#include "ichceditordialog.h"
#include "icvirtualhost.h"
#include "icmacrosubroutine.h"
#include "icparameterssave.h"

#include <QDebug>

ICHCParametersFrame::ICHCParametersFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCParametersFrame),
    isCopy_(false),
    isModified_(false),
    recordsDir_("records"),
    subsDir_("subs"),
    oldRecordName_(tr("None")),
    oldSubNum_(0),
    doesRecordEdited_(false),
    oldContentName_(tr("None"))
{
    ui->setupUi(this);

    InitSignals();
//    ResetHostParam();
}

ICHCParametersFrame::~ICHCParametersFrame()
{
    delete ui;
}

void ICHCParametersFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICHCParametersFrame::showEvent(QShowEvent * event)
{
//    editContent_ = ICMold::CurrentMold()->MoldContent();
    UpdateFileInfo_();
    ui->moldContentListWidget->clear();
    QFrame::showEvent(event);
}

void ICHCParametersFrame::hideEvent(QHideEvent * event)
{
    isCopy_ = false;
    if(isModified_)
    {
        if(oldContentName_ != tr("None"))
        {
            if(doesRecordEdited_)
            {
                ICMold::CurrentMold()->SetMoldContent(editContent_);
                ICMold::CurrentMold()->SaveMoldFile();
                ICInstructParam::Instance()->UpdateHostParam();
            }
            else
            {
                ICMacroSubroutine::Instance()->SaveMacroSubroutieFile(ui->subroutinesComboBox->currentIndex() - 1);
            }
        }
        ICVirtualHost::GlobalVirtualHost()->ReConfigure();
        isModified_ = false;
    }
    oldContentName_ = tr("None");
    oldRecordName_ = tr("None");
    oldSubNum_ = 0;
    QFrame::hideEvent(event);
}

void ICHCParametersFrame::on_copyToolButton_clicked()
{

    QList<QListWidgetItem *> selectedItems = ui->moldContentListWidget->selectedItems();
    if(selectedItems.isEmpty())
    {
        return;
    }
    ui->moldContentListWidget->setCurrentItem(selectedItems.first());
    int copyRow = ui->moldContentListWidget->currentRow();
    if(copyRow != -1)
    {
        moldItem_ = editContent_.at(copyRow);
        ui->editedCommandLabel->setText(ui->moldContentListWidget->item(
                copyRow)->text());
        isCopy_ = true;
    }
}

void ICHCParametersFrame::on_insertToolButton_clicked()
{
//    QList<QListWidgetItem *> selectedItems = ui->moldContentListWidget->selectedItems();
//    if(selectedItems.isEmpty())
//    {
//        return;
//    }
//    ui->moldContentListWidget->setCurrentItem(selectedItems.first());
//    int insertedRow = ui->moldContentListWidget->currentRow();
//    if(insertedRow != -1 && isCopy_)
//    {
//        moldItem_.SetSeq(insertedRow);
//        moldItem_.SetNVal(editContent_.at(insertedRow).NVal());

//        QList<ICMoldItem>::Iterator moldIterator = editContent_.begin() + insertedRow;
//        QList<ICMoldItem>::Iterator priorMoldIterator = moldIterator - 1;
//        moldIterator->SetNVal(moldIterator->NVal());

//        if(moldIterator == editContent_.begin())
//        {
//            editContent_.insert(insertedRow, moldItem_);
//        }
//        else
//        {
//            if(moldIterator->NVal() == priorMoldIterator->NVal())
//            {
//                editContent_.insert(insertedRow, moldItem_);
//            }
//            else
//            {
//                for(; moldIterator != editContent_.end() - 1; ++moldIterator)
//                {
//                    moldIterator->SetNVal(moldIterator->NVal() + 1);
//                }
//                editContent_.insert(insertedRow, moldItem_);
//            }
//        }

//        editContent_.last().SetNVal(editContent_.at(editContent_.size() - 2).NVal() + 1);

//        UpdateSumSep();

//        UpdateMoldContent(ICInstructParam::Instance()->ContentParam(editContent_));
//        ui->moldContentListWidget->item(insertedRow + 1)->setSelected(true);

//        isModified_ = true;
//    }
}

void ICHCParametersFrame::on_replacementToolButton_clicked()
{
    QList<QListWidgetItem *> selectedItems = ui->moldContentListWidget->selectedItems();
    if(selectedItems.isEmpty())
    {
        return;
    }
    ui->moldContentListWidget->setCurrentItem(selectedItems.first());
    int replacedRow = ui->moldContentListWidget->currentRow();
    if(replacedRow == ui->moldContentListWidget->count() - 1)
    {
        return;
    }
    if(replacedRow != -1 && isCopy_)
    {
        ReplaceRow(replacedRow);
        isModified_ = true;
    }
}

void ICHCParametersFrame::on_deleteToolButton_clicked()
{
//    QList<QListWidgetItem *> selectedItems = ui->moldContentListWidget->selectedItems();
//    if(selectedItems.isEmpty())
//    {
//        return;
//    }
//    ui->moldContentListWidget->setCurrentItem(selectedItems.first());
//    int deleteRow = ui->moldContentListWidget->currentRow();
//    if(deleteRow == ui->moldContentListWidget->count() - 1)
//    {
//        return;
//    }
//    if(deleteRow != -1)
//    {
//        QList<ICMoldItem>::Iterator moldIterator = editContent_.begin() + deleteRow;
//        QList<ICMoldItem>::Iterator nextMoldIterator = moldIterator + 1;
//        moldIterator->SetNVal(moldIterator->NVal());

//        if(moldIterator->NVal() == nextMoldIterator->NVal()
//            || (moldIterator->NVal() == 0 && nextMoldIterator->NVal() == 1))
//        {
//            editContent_.removeAt(deleteRow);
//        }
//        else
//        {
//            for(; moldIterator != editContent_.end(); ++moldIterator)
//            {
//                moldIterator->SetNVal(moldIterator->NVal() - 1);
//            }
//            editContent_.removeAt(deleteRow);
//        }

//        UpdateSumSep();

//        UpdateMoldContent(ICInstructParam::Instance()->ContentParam(editContent_));
//        ui->moldContentListWidget->item(deleteRow)->setSelected(true);;

//        isModified_ = true;
//    }
}

void ICHCParametersFrame::on_editToolButton_clicked()
{
    QList<QListWidgetItem *> selectedItems = ui->moldContentListWidget->selectedItems();
    if(selectedItems.isEmpty())
    {
        return;
    }
    ui->moldContentListWidget->setCurrentItem(selectedItems.first());
    int currentRow = ui->moldContentListWidget->currentRow();
    if(currentRow == ui->moldContentListWidget->count() - 1)
    {
        return;
    }
    if(currentRow != -1)
    {
        ICHCEditorDialog::Instance()->ResetParamInDialog(editContent_.at(currentRow));
        ICHCEditorDialog::Instance()->exec();
    }
}

void ICHCParametersFrame::UpdateCurrentMoldItem(ICMoldItem moldItem)
{
    int currentRow = ui->moldContentListWidget->currentRow();
    editContent_.replace(currentRow, moldItem);

    ui->moldContentListWidget->item(currentRow)->setText(
            ICInstructParam::Instance()->ConvertCommandStr(moldItem));
    isModified_ = true;
}

void ICHCParametersFrame::ResetHostParam()
{
    UpdateMoldContent(ICInstructParam::Instance()->CommandStrList());
}

void ICHCParametersFrame::UpdateMoldContent(const QStringList & commadnStrList)
{
    ui->moldContentListWidget->clear();
    ui->moldContentListWidget->addItems(commadnStrList);
}

void ICHCParametersFrame::UpdateSumSep()
{
//    QList<ICMoldItem>::Iterator moldIterator = editContent_.begin();

//    for(int i = 0; moldIterator != editContent_.end(); ++moldIterator)
//    {
////        moldIterator->SetSeq(i);
//        moldIterator->ReSum();
//    }
    ICMold::MoldReSum(editContent_);
}

void ICHCParametersFrame::ReplaceRow(int row)
{
//    moldItem_.SetSeq(row);
//    moldItem_.SetNVal(editContent_.at(row).NVal());
//    moldItem_.ReSum();

//    editContent_.replace(row, moldItem_);

//    UpdateMoldContent(ICInstructParam::Instance()->ContentParam(editContent_));
//    ui->moldContentListWidget->item(row)->setSelected(true);
}

void ICHCParametersFrame::InitSignals()
{
    connect(ICHCEditorDialog::Instance(),
            SIGNAL(EnterComplete(ICMoldItem)),
            this,
            SLOT(UpdateCurrentMoldItem(ICMoldItem)));
//    connect(ICInstructParam::Instance(),
//            SIGNAL(HostParamChanged()),
//            this,
//            SLOT(ResetHostParam()));
}

void ICHCParametersFrame::UpdateFileInfo_()
{
    ui->recordsComboBox->clear();
    ui->recordsComboBox->addItem(ICParametersSave::Instance()->MoldName(""));
//    ui->recordsComboBox->addItem(tr("None"));
//    ui->recordsComboBox->addItems(recordsDir_.entryList(QStringList()<<"*.act", QDir::Files | QDir::NoDotAndDotDot));

    ui->subroutinesComboBox->clear();
    ui->subroutinesComboBox->addItem(tr("None"));
    ui->subroutinesComboBox->addItems(subsDir_.entryList(QDir::Files | QDir::NoDotAndDotDot));
}

void ICHCParametersFrame::on_recordsComboBox_activated(const QString &fileName)
{
    if(oldContentName_ == fileName)
    {
        return;
    }
    QString pathName = recordsDir_.filePath(fileName);
    if(oldContentName_ != tr("None"))
    {
        if(!doesRecordEdited_)
        {
            if(QMessageBox::question(this,
                                     tr("Save Modify"),
                                     tr("Do you want to save what you have been edited?"),
                                     QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
            {
                ICMacroSubroutine::Instance()->SetSubRoutine(editContent_, ui->subroutinesComboBox->currentIndex() - 1);
                ICMacroSubroutine::Instance()->SaveMacroSubroutieFile(ui->subroutinesComboBox->currentIndex() - 1);
            }
//            moldFile_->ReadMoldFile(pathName, false);
//            UpdateMoldContent(ICInstructParam::Instance()->CommandStrList());
//            editContent_ = ICMold::CurrentMold()->MoldContent();

        }
        else
        {
            if(QMessageBox::question(this,
                                     tr("Save Modify"),
                                     tr("Do you want to save what you have been edited?"),
                                     QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
            {
                ICMold::CurrentMold()->SaveMoldFile(false);
            }
        }
        UpdateMoldContent(ICInstructParam::Instance()->CommandStrList());
        editContent_ = ICMold::CurrentMold()->MoldContent();
    }
    else
    {
        UpdateMoldContent(ICInstructParam::Instance()->CommandStrList());
        editContent_ = ICMold::CurrentMold()->MoldContent();
    }

    End:
    oldContentName_ = fileName;
    doesRecordEdited_ = true;
}


void ICHCParametersFrame::on_subroutinesComboBox_activated(int index)
{
    if(oldContentName_ == ui->subroutinesComboBox->currentText())
    {
        return;
    }
    if(oldContentName_ != tr("None"))
    {
        if(doesRecordEdited_)
        {
            if(QMessageBox::question(this,
                                     tr("Save Modify"),
                                     tr("Do you want to save what you have been edited?"),
                                     QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
            {
//                ICMold::CurrentMold()->SetMoldContent(editContent_);
                ICMold::CurrentMold()->SaveMoldFile(false);
            }
            if(index == 0)
            {
                ui->moldContentListWidget->clear();
                editContent_.clear();;
            }
            else
            {
                UpdateMoldContent(ICInstructParam::Instance()->ContentParam(ICMacroSubroutine::Instance()->SubRoutine(index - 1)));
                editContent_ = ICMacroSubroutine::Instance()->SubRoutine(index - 1);
            }
        }
        else
        {
            if(oldSubNum_ == index)
            {
                goto End;
            }
            if(QMessageBox::question(this,
                                     tr("Save Modify"),
                                     tr("Do you want to save what you have been edited?"),
                                     QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
            {
                ICMacroSubroutine::Instance()->SetSubRoutine(editContent_, oldSubNum_);
                ICMacroSubroutine::Instance()->SaveMacroSubroutieFile(oldSubNum_);
            }
            oldSubNum_ = index;
            if(index != 0)
            {
                UpdateMoldContent(ICInstructParam::Instance()->ContentParam(ICMacroSubroutine::Instance()->SubRoutine(index - 1)));
                editContent_ = ICMacroSubroutine::Instance()->SubRoutine(index - 1);
            }
            else
            {
                ui->moldContentListWidget->clear();
                editContent_.clear();
            }
        }
    }
    else
    {
        if(index != 0)
        {
           UpdateMoldContent(ICInstructParam::Instance()->ContentParam(ICMacroSubroutine::Instance()->SubRoutine(index - 1)));
           editContent_ = ICMacroSubroutine::Instance()->SubRoutine(index - 1);
        }
        else
        {
            ui->moldContentListWidget->clear();
            editContent_.clear();
        }
    }

    End:
    oldContentName_ = ui->subroutinesComboBox->currentText();
    doesRecordEdited_ = false;
}
