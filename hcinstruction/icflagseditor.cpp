#include "icflagseditor.h"
#include "ui_icflagseditor.h"

ICFlagsEditor::ICFlagsEditor(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICFlagsEditor)
{
    ui->setupUi(this);
//    flags_<<ICMoldUIItem::Group
//          <<ICMoldUIItem::GroupEnd
//          <<ICMoldUIItem::Parallel
//          <<ICMoldUIItem::ParallelEnd
//          <<ICMoldUIItem::Sync
//          <<ICMoldUIItem::SyncEnd;

    ICMoldItem item;
    ICSubMoldUIItem subItem;
    item.SetAction(ICMold::ACTParallel);
    item.SetSubNum(255);
    mainArmInsideGet_.SetBaseItem(item);
    mainArmOutsideGet_.SetBaseItem(item);
    subArmInsideGet_.SetBaseItem(item);
    subArmOUtsideGet_.SetBaseItem(item);

    item.SetAction(ICMold::GY);
    item.SetSubNum(0);
    item.SetPos(0);
    item.SetDVal(5);
    item.SetSVal(50);
    subItem.SetBaseItem(item);
    subItem.SetSyncItem(true);
    mainArmInsideGet_.AddSubMoldUIItem(subItem);
    mainArmOutsideGet_.AddSubMoldUIItem(subItem);
    item.SetAction(ICMold::ACTVICEDOWN);
    item.SetSVal(0);
    subItem.SetBaseItem(item);
    subArmInsideGet_.AddSubMoldUIItem(subItem);
    subArmOUtsideGet_.AddSubMoldUIItem(subItem);

    item.SetAction(ICMold::ACTMAINFORWARD);
    item.SetSubNum(1);
    item.SetPos(0);
    item.SetDVal(5);
    item.SetSVal(50);
    subItem.SetBaseItem(item);
    subItem.SetSyncItem(true);
    mainArmInsideGet_.AddSubMoldUIItem(subItem);
    mainArmOutsideGet_.AddSubMoldUIItem(subItem);
    item.SetAction(ICMold::ACTVICEFORWARD);
    item.SetSVal(0);
    subItem.SetBaseItem(item);
    subArmInsideGet_.AddSubMoldUIItem(subItem);
    subArmOUtsideGet_.AddSubMoldUIItem(subItem);

    item.SetClip(ICMold::ACTCLIP1ON);
    item.SetSubNum(2);
    item.SetPos(0);
    item.SetDVal(5);
    item.SetSVal(0);
    subItem.SetBaseItem(item);
    subItem.SetSyncItem(true);
    mainArmInsideGet_.AddSubMoldUIItem(subItem);
    mainArmOutsideGet_.AddSubMoldUIItem(subItem);
    subArmInsideGet_.AddSubMoldUIItem(subItem);
    subArmOUtsideGet_.AddSubMoldUIItem(subItem);

    item.SetAction(ICMold::ACTMAINBACKWARD);
    item.SetSubNum(3);
    item.SetPos(0);
    item.SetDVal(5);
    item.SetSVal(50);
    subItem.SetBaseItem(item);
    subItem.SetSyncItem(true);
    mainArmInsideGet_.AddSubMoldUIItem(subItem);
    mainArmOutsideGet_.AddSubMoldUIItem(subItem);
    item.SetAction(ICMold::ACTVICEBACKWARD);
    item.SetSVal(0);
    subItem.SetBaseItem(item);
    subArmInsideGet_.AddSubMoldUIItem(subItem);
    subArmOUtsideGet_.AddSubMoldUIItem(subItem);

    item.SetAction(ICMold::GY);
    item.SetSubNum(4);
    item.SetPos(0);
    item.SetDVal(5);
    item.SetSVal(50);
    subItem.SetBaseItem(item);
    subItem.SetSyncItem(true);
    mainArmInsideGet_.AddSubMoldUIItem(subItem);
    mainArmOutsideGet_.AddSubMoldUIItem(subItem);
    item.SetAction(ICMold::ACTVICEUP);
    item.SetSVal(0);
    subItem.SetBaseItem(item);
    subArmInsideGet_.AddSubMoldUIItem(subItem);
    subArmOUtsideGet_.AddSubMoldUIItem(subItem);

}

ICFlagsEditor::~ICFlagsEditor()
{
    delete ui;
}

void ICFlagsEditor::changeEvent(QEvent *e)
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

QList<ICMoldItem> ICFlagsEditor::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
//    ICMoldItem item;
//    item.SetAction(ICMold::ACTParallel);
//    item.SetSubNum(255);
//    ret.append(item);

//    item.SetAction(ICMold::GY);
//    item.SetSubNum(0);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);

//    item.SetClip(ICMold::ACTCLIP5ON);
//    item.SetSubNum(0);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(0);
//    ret.append(item);

//    item.SetAction(ICMold::GX);
//    item.SetSubNum(1);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);

//    item.SetAction(ICMold::GY);
//    item.SetSubNum(1);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);

//    item.SetAction(ICMold::GZ);
//    item.SetSubNum(1);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);

//    item.SetAction(ICMold::GY);
//    item.SetSubNum(2);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);

//    item.SetClip(ICMold::ACTCLIP5OFF);
//    item.SetSubNum(2);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(0);
//    ret.append(item);

//    item.SetAction(ICMold::GX);
//    item.SetSubNum(3);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);

//    item.SetAction(ICMold::GY);
//    item.SetSubNum(3);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);

//    item.SetAction(ICMold::GZ);
//    item.SetSubNum(3);
//    item.SetPos(0);
//    item.SetDVal(5);
//    item.SetSVal(50);
//    ret.append(item);
    return ret;
}

ICTopMoldUIItem ICFlagsEditor::CreateTopUIItem() const
{
    const int currentIndex = ui->flagListWidget->currentRow();
    if(currentIndex == 0)
    {
        return mainArmInsideGet_;
    }
    else if(currentIndex == 1)
    {
        return subArmInsideGet_;
    }
    else if(currentIndex == 2)
    {
        return mainArmOutsideGet_;
    }
    else if(currentIndex == 3)
    {
        return subArmOUtsideGet_;
    }
    return ICTopMoldUIItem();
}
