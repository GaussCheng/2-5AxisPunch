#include "icprogramguidepage.h"
#include "ui_icprogramguidepage.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"


ICProgramGuidePage::ICProgramGuidePage(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICProgramGuidePage),
    pageIndex_(0)
{
    ui->setupUi(this);
    axisWidgets_.append(QList<QWidget*>()<<ui->gx1Label<<ui->x1Edit<<ui->x1Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gy1Label<<ui->y1Edit<<ui->y1Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gzLabel<<ui->zEdit<<ui->zBox);
    axisWidgets_.append(QList<QWidget*>()<<ui->gx2Label<<ui->x2Edit<<ui->x2Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gy2Label<<ui->y2Edit<<ui->y2Box);
    axisWidgets_.append(QList<QWidget*>()<<ui->gaLabel<<ui->aEdit<<ui->aBox);
    axisWidgets_.append(QList<QWidget*>()<<ui->gbLabel<<ui->bEdit<<ui->bBox);
    axisWidgets_.append(QList<QWidget*>()<<ui->gcLabel<<ui->cEdit<<ui->cBox);
    axisWidgets_.append(QList<QWidget*>()<<ui->releaseOutletLabel<<ui->outletBox);
    posEdits_<<ui->x1Edit<<ui->y1Edit<<ui->zEdit<<ui->x2Edit<<ui->y2Edit
            <<ui->aEdit<<ui->bEdit<<ui->cEdit;
    limitEdits_<<ui->x1Box<<ui->y1Box<<ui->zBox<<ui->x2Box<<ui->y2Box
              <<ui->aBox<<ui->bBox<<ui->cBox;

    for(int i = 0; i != 8; ++i)
    {
        posValidators_[i].setBottom(0);
        axis_[i].mode = 0;
        axis_[i].standbyPos = 0;
        axis_[i].getPos = 0;
        axis_[i].releaseProductPos = 0;
        axis_[i].releaseOutletPos = 0;
    }
    /*
    * 当x2y2c轴选择为气动时初始化起点位置的值为后退上升垂直
    */
    UpdateAxisDefine_();
    if(axis_[C_AXIS].mode == AXIS_PNEUMATIC)
    {
        axis_[C_AXIS].standbyPos = 1;
    }
    if(axis_[C_AXIS].mode == AXIS_SERVO)
    {
        axis_[C_AXIS].standbyPos = 0 ;
    }
    if(axis_[X2_AXIS].mode == AXIS_PNEUMATIC)
    {
        axis_[X2_AXIS].standbyPos = 1 ;
    }
    if(axis_[Y2_AXIS].mode == AXIS_PNEUMATIC)
    {
        axis_[Y2_AXIS].standbyPos = 1 ;
    }
    for(int i = 0; i != posEdits_.size(); ++i)
    {
        posEdits_[i]->SetDecimalPlaces(1);
        posEdits_[i]->setValidator(&posValidators_[i]);
    }
    QList<int> temp;
    temp<<ICMold::ACTMAINFORWARD<<ICMold::ACTMAINBACKWARD;
    limitActionMap_.insert(axis_ + 0, temp);
    temp.clear();
    temp<<ICMold::ACTMAINUP<<ICMold::ACTMAINDOWN;
    limitActionMap_.insert(axis_ + 1, temp);
    temp.clear();
    temp<<ICMold::ACTCOMEIN<<ICMold::ACTGOOUT;
    limitActionMap_.insert(axis_ + 2, temp);
    temp.clear();
    temp<<ICMold::ACTVICEFORWARD<<ICMold::ACTVICEBACKWARD;
    limitActionMap_.insert(axis_ + 3, temp);
    temp.clear();
    temp<<ICMold::ACTVICEDOWN<<ICMold::ACTVICEUP;
    limitActionMap_.insert(axis_ + 4, temp);
    temp.clear();
    temp<<ICMold::ACT_PoseHori2<<ICMold::ACT_PoseVert2;
    limitActionMap_.insert(axis_ + 5, temp);
    temp.clear();
    temp<<ICMold::ACTPOSEHORI<<ICMold::ACTPOSEVERT;
    limitActionMap_.insert(axis_ + 6, temp);
    temp.clear();
    temp<<ICMold::ACTPOSEHORI<<ICMold::ACTPOSEVERT;
    limitActionMap_.insert(axis_ + 7, temp);
    temp.clear();

    fixtureOnAction_<<ICMold::ACTCLIP1ON<<ICMold::ACTCLIP2ON<<ICMold::ACTCLIP3ON
                   <<ICMold::ACTCLIP4ON<<ICMold::ACTCLIP5ON<<ICMold::ACTCLIP6ON;
    fixtureOffAction_<<ICMold::ACTCLIP1OFF<<ICMold::ACTCLIP2OFF<<ICMold::ACTCLIP3OFF
                    <<ICMold::ACTCLIP4OFF<<ICMold::ACTCLIP5OFF<<ICMold::ACTCLIP6OFF;

    on_usedMainArmBox_toggled(ui->usedMainArmBox->isChecked());
    on_usedSubArmBox_toggled(ui->usedSubArmBox->isChecked());
    HideWidgets_(axisWidgets_[8]);
    ui->outRunningHorizonBox->hide();
    ui->inRunningHorizonBox->hide();

//#ifdef Q_WS_X11
//    UpdateAxisDefine_();
//#endif
}

ICProgramGuidePage::~ICProgramGuidePage()
{
    delete ui;
}

void ICProgramGuidePage::changeEvent(QEvent *e)
{
    ICInstructionEditorBase::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICProgramGuidePage::showEvent(QShowEvent *e)
{
    pageIndex_ = 0;
    ui->stackedWidget->setCurrentIndex(0);
    UpdateAxisDefine_();
    UpdatePageButton_();
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    posMaxs_[0] = host->SystemParameter(ICVirtualHost::SYS_X_Maxium).toInt();
    posMaxs_[1] = host->SystemParameter(ICVirtualHost::SYS_Y_Maxium).toInt();
    posMaxs_[2] = host->SystemParameter(ICVirtualHost::SYS_Z_Maxium).toInt();
    posMaxs_[3] = host->SystemParameter(ICVirtualHost::SYS_P_Maxium).toInt();
    posMaxs_[4] = host->SystemParameter(ICVirtualHost::SYS_Q_Maxium).toInt();
    posMaxs_[5] = host->SystemParameter(ICVirtualHost::SYS_A_Maxium).toInt();
    posMaxs_[6] = host->SystemParameter(ICVirtualHost::SYS_B_Maxium).toInt();
    posMaxs_[7] = host->SystemParameter(ICVirtualHost::SYS_C_Maxium).toInt();
    for(int i = 0; i != 8; ++i)
    {
        posValidators_[i].setTop(posMaxs_[i]);
    }
    ICInstructionEditorBase::showEvent(e);
}

void ICProgramGuidePage::SyncStatusImpl(const QList<ICMoldItem> &items)
{
    Q_UNUSED(items)

}

QList<ICMoldItem> ICProgramGuidePage::CreateCommandImpl() const
{
    ICMoldItem item;
    QList<ICMoldItem> ret;
    bool isMainArmUsed = ui->usedMainArmBox->isChecked();
    bool isSubArmUsed = ui->usedSubArmBox->isChecked();
    int stepNum = 0;
    item.SetSVal(80);
    item.SetDVal(0);
    item.SetNum(stepNum++);
    item.SetSubNum(255);
    for(int i = 0; i != 8; ++i )//standby step
    {
        if(SetAxisICMoldItem_(&item, axis_ + i, STANDBY_SETTING))
        {
            ret.append(item);
        }
    }
    /********BUG#205**C轴选无，也必须在起点位置显示姿势垂直1********/
    if(axis_[7].mode == AXIS_NONE)
    {
        item.SetAction(ICMold::ACTPOSEVERT);
        ret.append(item);
    }
    /*****************************************/
    /*add wait for mold-open step*/
    item.SetNum(stepNum++);
    item.SetSVal(1);
    item.SetDVal(0);
    item.SetPos(0);
    item.SetAction(ICMold::ACT_WaitMoldOpened);
    ret.append(item);


    /*donw to get product*/
    /*****************BUG#133*******************/
    if(!ui->cBox->isHidden())
    {
        /***************BUG#182******************/
        if(!isMainArmUsed && !isSubArmUsed)
        {
        }
        else
        {
            item.SetNum(stepNum++);
            item.SetSVal(0);
            if(axis_[C_AXIS].getLimit == 0)
                item.SetAction(ICMold::ACTPOSEHORI);
            else
                item.SetAction(ICMold::ACTPOSEVERT);

            ret.append(item);
        }
    }
    item.SetSVal(80);
    item.SetDVal(0);
    if(!isMainArmUsed && !isSubArmUsed)
    {
  /***BUG#182**主副臂都不选择时，应出现模组结束（原来没有）*/
        item.SetNum(stepNum++);
        item.SetSVal(0);
        item.SetDVal(0);
        item.SetAction(ICMold::ACTEND);
        ret.append(item);
        return ret;
    }
    /*****当z轴为伺服******************/
    if(axis_[Z_AXIS].mode == AXIS_SERVO)
    {
        if(axis_[Z_AXIS].standbyPos != axis_[Z_AXIS].getPos)
        {
            item.SetNum(stepNum++);
            item.SetAction(axis_[Z_AXIS].action);
            /*BUG#132*/
            item.SetPos(axis_[Z_AXIS].getPos);
            ret.append(item);
        }
    }
    item.SetNum(stepNum++);
    if(isMainArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, GET_PRODUCT_SETTING))
        {
            ret.append(item);
        }
    }
    if(isSubArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, GET_PRODUCT_SETTING))
        {
            ret.append(item);
        }
    }
//    if(axis_[C_AXIS].mode == AXIS_SERVO)
//    {
//        item.SetNum(stepNum++);
//        item.SetSVal(80);
//        item.SetDVal(0);
//        if(SetAxisICMoldItem_(&item, axis_ + C_AXIS, GET_PRODUCT_SETTING))
//            ret.append(item);
//    }

    /*Ejector on*/
    item.SetNum(stepNum++);
    item.SetClip(ICMold::ACTEJECTON);
    item.SetDVal(10);
    ret.append(item);
    /*x axis forward to get product*/
    item.SetDVal(0);
    item.SetSVal(80);
    item.SetNum(stepNum++);
    if(isMainArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, GET_PRODUCT_SETTING))
        {
            ret.append(item);
        }
    }
    if(isSubArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, GET_PRODUCT_SETTING))
        {
            ret.append(item);
        }
    }

    /*clip product*/
    item.SetDVal(10);
    item.SetNum(stepNum++);
    if(isMainArmUsed)
    {
        item.SetClip(fixtureOnAction_.at(ui->productFixtureBox->currentIndex()));
        ret.append(item);
    }
    if(isSubArmUsed)
    {
        item.SetClip(fixtureOnAction_.at(ui->outletFixtureBox->currentIndex()));
        ret.append(item);
    }


    /*X axis backward*/
    item.SetDVal(0);
    item.SetSVal(80);
    item.SetNum(stepNum++);
    if(isMainArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, STANDBY_SETTING))
        {
            ret.append(item);
        }
    }
    if(isSubArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, STANDBY_SETTING))
        {
            ret.append(item);
        }
    }

    /*fixture check*/
    item.SetAction(ICMold::ACT_Cut);
    item.SetIFVal(1);
    item.SetNum(stepNum++);
    if(isMainArmUsed && ui->productCheck->isChecked())
    {
        item.SetSVal(ui->productFixtureBox->currentIndex());
        ret.append(item);
    }
    item.SetNum(stepNum++);
    if(isSubArmUsed && ui->outletCheck->isChecked())
    {
        item.SetSVal(ui->outletFixtureBox->currentIndex());
        ret.append(item);
    }

    /*go up*/
    item.SetSVal(80);
    item.SetDVal(0);
    item.SetNum(stepNum++);
    if(isMainArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, STANDBY_SETTING))
        {
            ret.append(item);
        }
    }
    if(isSubArmUsed)
    {
        if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, STANDBY_SETTING))
        {
            ret.append(item);
        }
    }


    /*go out*/
    item.SetNum(stepNum++);
    if(ui->outHorizontalBox->isChecked())
    {
        item.SetAction(ICMold::ACTPOSEHORI);
    }
    else if(ui->outVerticalBox->isChecked())
    {
        item.SetAction(ICMold::ACTPOSEVERT);
    }
    else if(ui->outRunningHorizonBox->isChecked())
    {
        item.SetAction(ICMold::ACTPOSEHORI);
    }
    ret.append(item);

    item.SetNum(stepNum++);
    item.SetClip(ICMold::ACTCLSMDON);
    item.SetDVal(10);
    ret.append(item);

    item.SetNum(stepNum++);
    item.SetAction(ICMold::GZ);
    item.SetSVal(80);
    item.SetDVal(0);
    if(isMainArmUsed && isSubArmUsed)
    {
        if(axis_[Z_AXIS].releaseProductPos < axis_[Z_AXIS].releaseOutletPos)
        {
            /*to release product pos*/
            if(SetAxisICMoldItem_(&item, axis_ + Z_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);
            /******************************************/
            if(!ui->cBox->isHidden())
            {
                item.SetSVal(0);
                if(axis_[C_AXIS].releaseProductLimit == 0)
                    item.SetAction(ICMold::ACTPOSEHORI);
                else
                    item.SetAction(ICMold::ACTPOSEVERT);
                ret.append(item);
            }
            /******************************************/
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);
            if(ui->stackedEn->isChecked())
            {
                item.SetSVal(ui->stackGroup->TransThisTextToThisInt() - 1);
                item.SetClip(ICMold::ACTLAYOUTON);
                ret.append(item);
                item.SetSVal(80);
            }
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);

            if(!ui->cEdit->isHidden())
            {
                item.SetNum(stepNum++);
                item.SetSVal(80);
                item.SetDVal(0);
                if(SetAxisICMoldItem_(&item, axis_ + C_AXIS, RELEASE_PRODUCT_SETTING))
                    ret.append(item);
            }
            /*release product*/
            item.SetNum(stepNum++);
            item.SetSVal(0);
            item.SetDVal(10);
            item.SetClip(fixtureOffAction_.at(ui->productFixtureBox->currentIndex()));
            ret.append(item);

            /*Y1, back to standby*/
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, STANDBY_SETTING))
                ret.append(item);

            /*to release outlet pos*/
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, STANDBY_SETTING))
                ret.append(item);
            if(SetAxisICMoldItem_(&item, axis_ + Z_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);
            if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);


            /*release outlet*/
            item.SetNum(stepNum++);
            item.SetSVal(0);
            item.SetDVal(10);
            item.SetClip(fixtureOffAction_.at(ui->outletFixtureBox->currentIndex()));
            ret.append(item);

            /*X2, Y2, back to standby*/
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, STANDBY_SETTING))
                ret.append(item);
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, STANDBY_SETTING))
                ret.append(item);

        }
        else if(axis_[Z_AXIS].releaseProductPos > axis_[Z_AXIS].releaseOutletPos)
        {
            /*to release outlet pos*/
            if(SetAxisICMoldItem_(&item, axis_ + Z_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);
            /******************************************/
            if(!ui->cBox->isHidden())
            {
                item.SetSVal(0);
                if(axis_[C_AXIS].releaseProductLimit == 0)
                    item.SetAction(ICMold::ACTPOSEHORI);
                else
                    item.SetAction(ICMold::ACTPOSEVERT);
                ret.append(item);
            }
            /******************************************/

            if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);
            /*release outlet*/
            item.SetNum(stepNum++);
            item.SetSVal(0);
            item.SetDVal(10);
            item.SetClip(fixtureOffAction_.at(ui->outletFixtureBox->currentIndex()));
            ret.append(item);

            /*Y2, back to standby*/
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, STANDBY_SETTING))
                ret.append(item);



            /*to release product pos*/
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, STANDBY_SETTING))
                ret.append(item);
            if(SetAxisICMoldItem_(&item, axis_ + Z_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);         
//            /******************************************/
//            if(!ui->cBox->isHidden())
//            {
//                item.SetSVal(0);
//                if(axis_[C_AXIS].releaseProductLimit == 0)
//                    item.SetAction(ICMold::ACTPOSEHORI);
//                else
//                    item.SetAction(ICMold::ACTPOSEVERT);
//                ret.append(item);
//            }
//            /******************************************/
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);
            if(ui->stackedEn->isChecked())
            {
                item.SetSVal(ui->stackGroup->TransThisTextToThisInt() - 1);
                item.SetClip(ICMold::ACTLAYOUTON);
                ret.append(item);
                item.SetSVal(80);
            }
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);

            if(!ui->cEdit->isHidden())
            {
                item.SetNum(stepNum++);
                item.SetSVal(80);
                item.SetDVal(0);
                if(SetAxisICMoldItem_(&item, axis_ + C_AXIS, RELEASE_PRODUCT_SETTING))
                    ret.append(item);
            }
            /*release product*/
            item.SetNum(stepNum++);
            item.SetSVal(0);
            item.SetDVal(10);
            item.SetClip(fixtureOffAction_.at(ui->productFixtureBox->currentIndex()));
            ret.append(item);


            /*X2, Y2, back to standby*/
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, STANDBY_SETTING))
                ret.append(item);
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, STANDBY_SETTING))
                ret.append(item);

        }
        else
        {
            if(SetAxisICMoldItem_(&item, axis_ + Z_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);
            /******************************************/
            if(!ui->cBox->isHidden())
            {
                item.SetSVal(0);
                if(axis_[C_AXIS].releaseProductLimit == 0)
                    item.SetAction(ICMold::ACTPOSEHORI);
                else
                    item.SetAction(ICMold::ACTPOSEVERT);
                ret.append(item);
            }
            /******************************************/
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);
            if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);
            if(ui->stackedEn->isChecked())
            {
                item.SetSVal(ui->stackGroup->TransThisTextToThisInt() - 1);
                item.SetClip(ICMold::ACTLAYOUTON);
                ret.append(item);
                item.SetSVal(80);
            }
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);
            if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, RELEASE_OUTLET_SETTING))
                ret.append(item);

            if(!ui->cEdit->isHidden())
            {
                item.SetNum(stepNum++);
                item.SetSVal(80);
                item.SetDVal(0);
                if(SetAxisICMoldItem_(&item, axis_ + C_AXIS, RELEASE_PRODUCT_SETTING))
                    ret.append(item);
            }

            item.SetNum(stepNum++);
            item.SetSVal(0);
            item.SetDVal(10);
            item.SetClip(fixtureOffAction_.at(ui->productFixtureBox->currentIndex()));
            ret.append(item);
            item.SetClip(fixtureOffAction_.at(ui->outletFixtureBox->currentIndex()));
            ret.append(item);

            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, STANDBY_SETTING))
                ret.append(item);
            if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, STANDBY_SETTING))
                ret.append(item);
            item.SetNum(stepNum++);
            if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, STANDBY_SETTING))
                ret.append(item);
            if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, STANDBY_SETTING))
                ret.append(item);

        }

    }
    else if(isMainArmUsed)
    {
        /*to release product pos*/
        if(SetAxisICMoldItem_(&item, axis_ + Z_AXIS, RELEASE_PRODUCT_SETTING))
            ret.append(item);
        /*****************************************/
        if(!ui->cBox->isHidden())
        {
            item.SetSVal(0);
            if(axis_[C_AXIS].releaseProductLimit == 0)
                item.SetAction(ICMold::ACTPOSEHORI);
            else
                item.SetAction(ICMold::ACTPOSEVERT);
            ret.append(item);
        }
        /*****************************************/
        if(ui->stackedEn->isChecked())
        {
            item.SetSVal(ui->stackGroup->TransThisTextToThisInt() - 1);
            item.SetClip(ICMold::ACTLAYOUTON);
            ret.append(item);
            item.SetSVal(80);
        }
        item.SetNum(stepNum++);
        item.SetSVal(80);
        item.SetDVal(0);
        if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, RELEASE_PRODUCT_SETTING))
            ret.append(item);
        item.SetNum(stepNum++);
        if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, RELEASE_PRODUCT_SETTING))
            ret.append(item);

        if(!ui->cEdit->isHidden())
        {
            item.SetNum(stepNum++);
            item.SetSVal(80);
            item.SetDVal(0);
            if(SetAxisICMoldItem_(&item, axis_ + C_AXIS, RELEASE_PRODUCT_SETTING))
                ret.append(item);
        }

        /*release product*/
        item.SetNum(stepNum++);
        item.SetSVal(0);
        item.SetDVal(10);
        item.SetClip(fixtureOffAction_.at(ui->productFixtureBox->currentIndex()));
        ret.append(item);


        /*X1, Y1, back to standby*/
        item.SetNum(stepNum++);
        item.SetSVal(80);
        item.SetDVal(0);
        if(SetAxisICMoldItem_(&item, axis_ + Y1_AXIS, STANDBY_SETTING))
            ret.append(item);
        item.SetNum(stepNum++);
        if(SetAxisICMoldItem_(&item, axis_ + X1_AXIS, STANDBY_SETTING))
            ret.append(item);
    }
    else if(isSubArmUsed)
    {
        /*to release outlet pos*/
        if(SetAxisICMoldItem_(&item, axis_ + Z_AXIS, RELEASE_OUTLET_SETTING))
            ret.append(item);
        if( SetAxisICMoldItem_(&item, axis_ + X2_AXIS, RELEASE_OUTLET_SETTING))
            ret.append(item);
        item.SetNum(stepNum++);
        if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, RELEASE_OUTLET_SETTING))
            ret.append(item);

        /*release outlet*/
        item.SetNum(stepNum++);
        item.SetSVal(0);
        item.SetDVal(10);
        item.SetClip(fixtureOffAction_.at(ui->outletFixtureBox->currentIndex()));
        ret.append(item);

        /*X2, Y2, back to standby*/
        item.SetNum(stepNum++);
        item.SetSVal(80);
        item.SetDVal(0);
        if(SetAxisICMoldItem_(&item, axis_ + Y2_AXIS, STANDBY_SETTING))
            ret.append(item);
        item.SetNum(stepNum++);
        if(SetAxisICMoldItem_(&item, axis_ + X2_AXIS, STANDBY_SETTING))
            ret.append(item);
    }
    if(ui->inHorizontalBox->isChecked())
    {
        item.SetNum(stepNum++);
        item.SetAction(ICMold::ACTPOSEHORI);
        ret.append(item);
    }
    else if(ui->inVerticalBox->isChecked())
    {
        item.SetNum(stepNum++);
        item.SetAction(ICMold::ACTPOSEVERT);
        ret.append(item);
    }
    item.SetNum(stepNum++);
    item.SetSVal(0);
    item.SetDVal(0);
    item.SetAction(ICMold::ACTEND);
    ret.append(item);
    return ret;

}

void ICProgramGuidePage::UpdateAxisDefine_()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    int currentAxis = host->SystemParameter(ICVirtualHost::SYS_Config_Arm).toInt();
    if(axisDefine_ != currentAxis)
    {
        axisDefine_ = currentAxis;
        for(int i = 0 ; i != axisWidgets_.size(); ++i)
        {
            HideWidgets_(axisWidgets_[i]);
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[0]);
            axis_[0].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[0]);
            ui->x1Edit->hide();
            axis_[0].mode = AXIS_PNEUMATIC;
            //            ui->x1pLabel->hide();
        }
        else
        {
            ShowWidgets_(axisWidgets_[0]);
            ui->x1Box->hide();
            axis_[0].mode = AXIS_SERVO;
            axis_[0].action = ICMold::GX;
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[1]);
            axis_[1].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[1]);
            ui->y1Edit->hide();
            axis_[1].mode = AXIS_PNEUMATIC;
        }
        else
        {
            ShowWidgets_(axisWidgets_[1]);
            ui->y1Box->hide();
            axis_[1].mode = AXIS_SERVO;
            axis_[1].action = ICMold::GY;
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[2]);
            axis_[2].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[2]);
            ui->zEdit->hide();
            axis_[2].mode = AXIS_PNEUMATIC;
        }
        else
        {
            ShowWidgets_(axisWidgets_[2]);
            ui->zBox->hide();
            axis_[2].mode = AXIS_SERVO;
            axis_[2].action = ICMold::GZ;
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[3]);
            axis_[3].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[3]);
            ui->x2Edit->hide();
            axis_[3].mode = AXIS_PNEUMATIC;
        }
        else
        {
            ShowWidgets_(axisWidgets_[3]);
            ui->x2Box->hide();
            axis_[3].mode = AXIS_SERVO;
            axis_[3].action = ICMold::GP;
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[4]);
            axis_[4].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[4]);
            ui->y2Edit->hide();
            axis_[4].mode = AXIS_PNEUMATIC;
        }
        else
        {
            ShowWidgets_(axisWidgets_[4]);
            ui->y2Box->hide();
            axis_[4].mode = AXIS_SERVO;
            axis_[4].action = ICMold::GQ;
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[5]);
            axis_[5].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[5]);
            ui->aEdit->hide();
            axis_[5].mode = AXIS_PNEUMATIC;
        }
        else
        {
            ShowWidgets_(axisWidgets_[5]);
            ui->aBox->hide();
            axis_[5].mode = AXIS_SERVO;
            axis_[5].action = ICMold::GA;
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[6]);
            axis_[6].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[6]);
            ui->bEdit->hide();
            axis_[6].mode = AXIS_PNEUMATIC;
        }
        else
        {
            ShowWidgets_(axisWidgets_[6]);
            ui->bBox->hide();
            axis_[6].mode = AXIS_SERVO;
            axis_[6].action = ICMold::GB;
        }

        if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) == ICVirtualHost::ICAxisDefine_None)
        {
            HideWidgets_(axisWidgets_[7]);
            axis_[7].mode = AXIS_NONE;
        }
        else if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) == ICVirtualHost::ICAxisDefine_Pneumatic)
        {
            ShowWidgets_(axisWidgets_[7]);
            ui->cEdit->hide();
            axis_[7].mode = AXIS_PNEUMATIC;
        }
        else
        {
            ShowWidgets_(axisWidgets_[7]);
            ui->cBox->hide();
            axis_[7].mode = AXIS_SERVO;
            axis_[7].action = ICMold::GC;
        }
    }
}

void ICProgramGuidePage::ShowWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->show();
    }
}

void ICProgramGuidePage::HideWidgets_(QList<QWidget *> &widgets)
{
    for(int i = 0; i != widgets.size(); ++i)
    {
        widgets[i]->hide();
    }
}

void ICProgramGuidePage::on_nextButton_clicked()
{
    ++pageIndex_;
    if(pageIndex_ == 1) //show for standby settings
    {
        ui->stackedWidget->setCurrentIndex(1);
        ShowForStandby_();
       // SaveAxis_(STANDBY_SETTING);
        UpdateAxisShow(STANDBY_SETTING);
        ui->cBox->setEnabled(true);
        ui->cEdit->setEnabled(true);
        SetAxisBoxEnabled_(true);
    }
    else if(pageIndex_ == 2)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->descrLabel->setText(tr("Get Position Settings"));
        on_usedMainArmBox_toggled(ui->usedMainArmBox->isChecked());
        on_usedSubArmBox_toggled(ui->usedSubArmBox->isChecked());
        ui->commonGroupBox->show();
        SetAxisBoxEnabled_(true);
        if(axis_[C_AXIS].mode == AXIS_SERVO)
            ui->cEdit->setEnabled(false);
        if(axis_[C_AXIS].mode == AXIS_PNEUMATIC)
            ui->cBox->setEnabled(false);
        SaveAxis_(STANDBY_SETTING);
        UpdateAxisShow(GET_PRODUCT_SETTING);
        ui->productGroupBox->setTitle(tr("Product"));
        ui->outletGroupBox->setTitle(tr("Outlet"));
    }
    else if(pageIndex_ == 3)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->descrLabel->setText(tr("Release Product Position Settings"));
        on_usedMainArmBox_toggled(ui->usedMainArmBox->isChecked());
        ui->outletGroupBox->hide();
        ui->cBox->setEnabled(true);
        ui->cEdit->setEnabled(true);
        SetAxisBoxEnabled_(true);
        if(!ui->usedMainArmBox->isChecked())
        {
            ui->commonGroupBox->hide();
        }
        else
        {
            ui->commonGroupBox->show();
        }
        SaveAxis_(GET_PRODUCT_SETTING);
        UpdateAxisShow(RELEASE_PRODUCT_SETTING);
    }
    else if(pageIndex_ == 4)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->descrLabel->setText(tr("Release Outlet Position Settings"));
        on_usedSubArmBox_toggled(ui->usedSubArmBox->isChecked());
        ui->productGroupBox->hide();
        ui->cEdit->setEnabled(false);
        ui->cBox->setEnabled(false);
        if(!ui->usedSubArmBox->isChecked())
        {
            ui->commonGroupBox->hide();
        }
        else
        {
            ui->commonGroupBox->show();
        }
        SaveAxis_(RELEASE_PRODUCT_SETTING);
        UpdateAxisShow(RELEASE_OUTLET_SETTING);
    }
    else if(pageIndex_ == 5)
    {
        ui->stackedWidget->setCurrentIndex(2);
        SaveAxis_(RELEASE_OUTLET_SETTING);
    }
    UpdatePageButton_();
}

void ICProgramGuidePage::on_preButton_clicked()
{
    --pageIndex_;
    if(pageIndex_ == 0)
        {
            ui->stackedWidget->setCurrentIndex(0);
            on_usedMainArmBox_toggled(ui->usedMainArmBox->isChecked());
            on_usedSubArmBox_toggled(ui->usedSubArmBox->isChecked());
           SaveAxis_(STANDBY_SETTING);
        }
    else if(pageIndex_ == 1) //show for standby settings
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->cBox->setEnabled(true);
        ui->cEdit->setEnabled(true);
        ShowForStandby_();
        SaveAxis_(GET_PRODUCT_SETTING);
        UpdateAxisShow(STANDBY_SETTING);
        SetAxisBoxEnabled_(true);
    }
    else if(pageIndex_ == 2)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->descrLabel->setText(tr("Get Position Settings"));
        on_usedMainArmBox_toggled(ui->usedMainArmBox->isChecked());
        on_usedSubArmBox_toggled(ui->usedSubArmBox->isChecked());
        ui->commonGroupBox->show();
        SetAxisBoxEnabled_(true);
        if(axis_[C_AXIS].mode == AXIS_SERVO)
            ui->cEdit->setEnabled(false);
        if(axis_[C_AXIS].mode == AXIS_PNEUMATIC)
            ui->cBox->setEnabled(false);
        SaveAxis_(RELEASE_PRODUCT_SETTING);
        UpdateAxisShow(GET_PRODUCT_SETTING);
    }
    else if(pageIndex_ == 3)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->descrLabel->setText(tr("Release Product Position Settings"));
        on_usedMainArmBox_toggled(ui->usedMainArmBox->isChecked());
        ui->outletGroupBox->hide();
        ui->cEdit->setEnabled(true);
        ui->cBox->setEnabled(true);
        if(!ui->usedMainArmBox->isChecked())
        {
            ui->commonGroupBox->hide();
        }
        else
        {
            ui->commonGroupBox->show();
        }
        SaveAxis_(RELEASE_OUTLET_SETTING);
        UpdateAxisShow(RELEASE_PRODUCT_SETTING);
        SetAxisBoxEnabled_(true);
    }
    else if(pageIndex_ == 4)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->descrLabel->setText(tr("Release Outlet Position Settings"));
        on_usedSubArmBox_toggled(ui->usedSubArmBox->isChecked());
    //    ui->productGroupBox->hide();
        if(axis_[C_AXIS].mode == AXIS_SERVO)
            ui->cEdit->setEnabled(false);
        if(axis_[C_AXIS].mode == AXIS_PNEUMATIC)
            ui->cBox->setEnabled(false);
        if(!ui->usedSubArmBox->isChecked())
        {
            ui->commonGroupBox->hide();
        }
        else
        {
            ui->commonGroupBox->show();
        }
        UpdateAxisShow(RELEASE_OUTLET_SETTING);
    }

    UpdatePageButton_();
}

void ICProgramGuidePage::UpdatePageButton_()
{
    switch(pageIndex_)
    {
    case 0: ui->preButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
        break;
    case 1:
    case 2:
    case 3:
    case 4: ui->preButton->setEnabled(true);
        ui->nextButton->setEnabled(true);
        break;
    case 5: ui->nextButton->setEnabled(false);break;
    default:break;

    }

}

void ICProgramGuidePage::ShowForStandby_()
{
    ui->descrLabel->setText(tr("Stanby Settings"));
    ui->commonGroupBox->show();
    ui->productGroupBox->setTitle("");
    ui->outletGroupBox->setTitle("");
    SetAxisBoxEnabled_(false);
    ui->cBox->setEnabled(true);
//    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX1) == ICVirtualHost::ICAxisDefine_Pneumatic)
//    {
//     //   ui->x1Box->setCurrentIndex(1);
//        SetAxis_(axis_, ui->x1Box->currentIndex(), STANDBY_SETTING);
//    }
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY1) == ICVirtualHost::ICAxisDefine_Pneumatic)
//    {
//      //  ui->y1Box->setCurrentIndex(0);
//        SetAxis_(axis_ + 1, ui->y1Box->currentIndex(), STANDBY_SETTING);
//    }
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisZ) == ICVirtualHost::ICAxisDefine_Pneumatic)
//    {
//      //  ui->zBox->setCurrentIndex(0);
//        SetAxis_(axis_ + 2, ui->zBox->currentIndex(), STANDBY_SETTING);
//    }
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisX2) == ICVirtualHost::ICAxisDefine_Pneumatic)
//    {
//       // ui->x2Box->setCurrentIndex(1);
//        SetAxis_(axis_ + 3, ui->x2Box->currentIndex(), STANDBY_SETTING);
//    }
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisY2) == ICVirtualHost::ICAxisDefine_Pneumatic)
//    {
//      //  ui->y2Box->setCurrentIndex(1);
//        SetAxis_(axis_ + 4, ui->y2Box->currentIndex(), STANDBY_SETTING);
//    }
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisA) == ICVirtualHost::ICAxisDefine_Servo)
//    {
////        HideWidgets_(axisWidgets_[5]);
////        ui->aEdit->SetThisIntToThisText(0);
//    }
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisB) == ICVirtualHost::ICAxisDefine_Servo)
//    {
////        HideWidgets_(axisWidgets_[6]);
////        ui->bEdit->SetThisIntToThisText(0);
//    }
//    if(host->AxisDefine(ICVirtualHost::ICAxis_AxisC) == ICVirtualHost::ICAxisDefine_Servo)
//    {
////        HideWidgets_(axisWidgets_[7]);
////        ui->cEdit->SetThisIntToThisText(0);
//    }
    if(ui->usedMainArmBox->isChecked())
        on_usedMainArmBox_toggled(true);
    if(ui->usedMainArmBox->isChecked())
        on_usedSubArmBox_toggled(true);
}

void ICProgramGuidePage::SetAxisBoxEnabled_(bool en)
{
    ui->x1Box->setEnabled(en);
    ui->y1Box->setEnabled(en);
    ui->zBox->setEnabled(en);
    ui->x2Box->setEnabled(en);
    ui->y2Box->setEnabled(en);
    ui->aBox->setEnabled(en);
    ui->bBox->setEnabled(en);
    ui->cBox->setEnabled(en);
}

void ICProgramGuidePage::SetAxis_(_ICAxis_ *axis, int pos, int setting)
{
    if(setting == STANDBY_SETTING)
    {
        axis->standbyPos = pos;
    }
    else if(setting == GET_PRODUCT_SETTING)
    {
        axis->getPos = pos;
    }
    else if(setting == RELEASE_PRODUCT_SETTING)
    {
        axis->releaseProductPos = pos;
    }
    else if(setting == RELEASE_OUTLET_SETTING)
    {
        axis->releaseOutletPos = pos;
    }
}
void ICProgramGuidePage::SaveAxis_(int setting)
{
    for(int i = 0; i != posEdits_.size(); ++i)
    {
        if(axis_[i].mode == AXIS_SERVO)
        {
            SetAxis_(axis_ + i, posEdits_.at(i)->TransThisTextToThisInt(), setting);
        }
        else if(axis_[i].mode == AXIS_PNEUMATIC)
        {
            SetAxis_(axis_ + i, limitEdits_.at(i)->currentIndex(), setting);
        }
    }
}

void ICProgramGuidePage::SetAxisPosEdit_(ICLineEditWithVirtualNumericKeypad *edit, _ICAxis_ *axis, int setting)
{
    if(setting == STANDBY_SETTING)
    {
        edit->SetThisIntToThisText(axis->standbyPos);
    }
    else if(setting == GET_PRODUCT_SETTING)
    {
        edit->SetThisIntToThisText(axis->getPos);
    }
    else if(setting == RELEASE_PRODUCT_SETTING)
    {
        edit->SetThisIntToThisText(axis->releaseProductPos);
    }
    else if(setting == RELEASE_OUTLET_SETTING)
    {
        edit->SetThisIntToThisText(axis->releaseOutletPos);
    }
}

void ICProgramGuidePage::SetAxisLimitEdit_(QComboBox *edit, _ICAxis_ *axis, int setting)
{
    if(setting == STANDBY_SETTING)
    {
        edit->setCurrentIndex(axis->standbyLimit);
    }
    else if(setting == GET_PRODUCT_SETTING)
    {
        edit->setCurrentIndex(axis->getLimit);
    }
    else if(setting == RELEASE_PRODUCT_SETTING)
    {
        edit->setCurrentIndex(axis->releaseProductLimit);
    }
    else if(setting == RELEASE_OUTLET_SETTING)
    {
        edit->setCurrentIndex(axis->releaseOutletLimit);
    }
}
void ICProgramGuidePage::UpdateAxisShow(int setting)
{
    for(int i = 0; i != posEdits_.size(); ++i)
    {
        if(axis_[i].mode == AXIS_SERVO)
        {
            SetAxisPosEdit_(posEdits_[i], axis_ + i, setting);
        }
        else if(axis_[i].mode == AXIS_PNEUMATIC)
        {
            SetAxisLimitEdit_(limitEdits_[i], axis_ + i, setting);
        }
    }
}

bool ICProgramGuidePage::SetAxisICMoldItem_(ICMoldItem *item, const _ICAxis_ *axis, int setting) const
{
    if(axis->mode == AXIS_SERVO)
    {
        item->SetAction(axis->action);
        if(setting == STANDBY_SETTING)
        {
            item->SetPos(axis->standbyPos);
        }
        else if(setting == GET_PRODUCT_SETTING)
        {
            item->SetPos(axis->getPos);
        }
        else if(setting == RELEASE_PRODUCT_SETTING)
        {
            item->SetPos(axis->releaseProductPos);
        }
        else if(setting == RELEASE_OUTLET_SETTING)
        {
            item->SetPos(axis->releaseOutletPos);
        }
        return true;
    }
    else if(axis->mode == AXIS_PNEUMATIC)
    {
        item->SetPos(0);
        if(setting == STANDBY_SETTING)
        {
            item->SetAction(limitActionMap_.value(axis).at(axis->standbyLimit));
        }
        else if(setting == GET_PRODUCT_SETTING)
        {
            item->SetAction(limitActionMap_.value(axis).at(axis->getLimit));
        }
        else if(setting == RELEASE_PRODUCT_SETTING)
        {
            item->SetAction(limitActionMap_.value(axis).at(axis->releaseProductLimit));
        }
        else if(setting == RELEASE_OUTLET_SETTING)
        {
            item->SetAction(limitActionMap_.value(axis).at(axis->releaseOutletLimit));
        }
        return true;
    }
    return false;
}

void ICProgramGuidePage::on_finishButton_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        SaveAxis_(pageIndex_);
    }
    emit GuideFinished();
}

void ICProgramGuidePage::on_usedMainArmBox_toggled(bool checked)
{
    if(checked)
    {
        ui->productGroupBox->show();
    }
    else
    {
        ui->productGroupBox->hide();
    }
    ui->productFixtureGroup->setEnabled(checked);
}

void ICProgramGuidePage::on_usedSubArmBox_toggled(bool checked)
{
    if(checked)
    {
        ui->outletGroupBox->show();
    }
    else
    {
        ui->outletGroupBox->hide();
    }
    ui->outletFixtureGroup->setEnabled(checked);
}

void ICProgramGuidePage::on_stackedEn_toggled(bool checked)
{
    ui->stackGroup->setEnabled(checked);
}

void ICProgramGuidePage::on_setInButton_clicked()
{
    ICVirtualHost *host = ICVirtualHost::GlobalVirtualHost();
    ui->x1Edit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::XPos).toInt());
    ui->y1Edit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::YPos).toInt());
    ui->zEdit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::ZPos).toInt());
    ui->x2Edit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::PPos).toInt());
    ui->y2Edit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::QPos).toInt());
    ui->aEdit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::APos).toInt());
    ui->bEdit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::BPos).toInt());
    ui->cEdit->SetThisIntToThisText(host->HostStatus(ICVirtualHost::CPos).toInt());
}

void ICProgramGuidePage::GuideKeyToActionCheck(int key)
{
    switch(key)
    {
    case ICKeyboard::VFB_X1Add:
    case ICKeyboard::VFB_X1Sub:
        ui->x1Box->setCurrentIndex(key == ICKeyboard::VFB_X1Add ? 0:1);
        break;
    case ICKeyboard::VFB_Y1Add:
    case ICKeyboard::VFB_Y1Sub:
        ui->y1Box->setCurrentIndex(key == ICKeyboard::VFB_Y1Add ? 1:0);
        break;
    case ICKeyboard::VFB_ZAdd:
    case ICKeyboard::VFB_ZSub:
        ui->zBox->setCurrentIndex(key == ICKeyboard::VFB_ZAdd ? 1:0);
        break;
    case ICKeyboard::VFB_X2Add:
    case ICKeyboard::VFB_X2Sub:
        ui->x2Box->setCurrentIndex(key == ICKeyboard::VFB_X2Add ? 0:1);
        break;
    case ICKeyboard::VFB_Y2Add:
    case ICKeyboard::VFB_Y2Sub:
        ui->y2Box->setCurrentIndex(key == ICKeyboard::VFB_Y2Add ?0:1);
        break;
    case ICKeyboard::VFB_AAdd:
    case ICKeyboard::VFB_ASub:
        ui->aBox->setCurrentIndex(key == ICKeyboard::VFB_AAdd ? 0:1);
        break;
    case ICKeyboard::VFB_BAdd:
    case ICKeyboard::VFB_BSub:
        ui->bBox->setCurrentIndex(key == ICKeyboard::VFB_BAdd ? 1:0);
        break;
    case ICKeyboard::VFB_Pose_Horizontal:
    case ICKeyboard::VFB_Pose_Vertical:
        ui->cBox->setCurrentIndex(key == ICKeyboard::VFB_Pose_Horizontal ? 1:0);
        break;

    }
}
