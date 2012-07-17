#include "icstackeditor.h"
#include "ui_icstackeditor.h"

#include "icmold.h"

struct StackGroup
{
    QCheckBox* check;
    QLabel* seq;
    QLabel* xPosition;
    QLabel* yPosition;
    QLabel* zPosition;
    QLabel* xNum;
    QLabel* yNum;
    QLabel* zNum;
    QLabel* xStep;
    QLabel* yStep;
    QLabel* zStep;
};

QList<StackGroup> stackGroups;

ICStackEditor::ICStackEditor(QWidget *parent) :
    ICInstructionEditorBase(parent),
    ui(new Ui::ICStackEditor)
{
    ui->setupUi(this);
    StackGroup group;
    group.check = ui->g1Check;
    group.seq = ui->g1Seq;
    group.xPosition = ui->g1XPosition;
    group.yPosition = ui->g1YPosition;
    group.zPosition = ui->g1ZPosition;
    group.xNum = ui->g1XNum;
    group.yNum = ui->g1YNum;
    group.zNum = ui->g1ZNum;
    group.xStep = ui->g1XStep;
    group.yStep = ui->g1YStep;
    group.zStep = ui->g1ZStep;
    stackGroups.append(group);
    group.check = ui->g2Check;
    group.seq = ui->g2Seq;
    group.xPosition = ui->g2XPosition;
    group.yPosition = ui->g2YPosition;
    group.zPosition = ui->g2ZPosition;
    group.xNum = ui->g2XNum;
    group.yNum = ui->g2YNum;
    group.zNum = ui->g2ZNum;
    group.xStep = ui->g2XStep;
    group.yStep = ui->g2YStep;
    group.zStep = ui->g2ZStep;
    stackGroups.append(group);
    group.check = ui->g3Check;
    group.seq = ui->g3Seq;
    group.xPosition = ui->g3XPosition;
    group.yPosition = ui->g3YPosition;
    group.zPosition = ui->g3ZPosition;
    group.xNum = ui->g3XNum;
    group.yNum = ui->g3YNum;
    group.zNum = ui->g3ZNum;
    group.xStep = ui->g3XStep;
    group.yStep = ui->g3YStep;
    group.zStep = ui->g3ZStep;
    stackGroups.append(group);
    group.check = ui->g4Check;
    group.seq = ui->g4Seq;
    group.xPosition = ui->g4XPosition;
    group.yPosition = ui->g4YPosition;
    group.zPosition = ui->g4ZPosition;
    group.xNum = ui->g4XNum;
    group.yNum = ui->g4YNum;
    group.zNum = ui->g4ZNum;
    group.xStep = ui->g4XStep;
    group.yStep = ui->g4YStep;
    group.zStep = ui->g4ZStep;
    stackGroups.append(group);
}

ICStackEditor::~ICStackEditor()
{
    delete ui;
}

void ICStackEditor::changeEvent(QEvent *e)
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

void ICStackEditor::showEvent(QShowEvent *e)
{
    StackGroup group;
    for(int i = 0; i != stackGroups.size(); ++i)
    {
        group = stackGroups[i];
        QList<int> stackParams = ICMold::CurrentMold()->StackParams(i);
        int seq = stackParams.at(ICMold::Seq);
        int seqH;
        int seqL;
        seqH = seq;
        seqL = seq & 0x000F;
        if(seqL == 0)
        {
            //        ui->xzyCheckBox->setChecked(true);
            group.seq->setText(tr("X->Z->Y"));
        }
        else if(seqL == 1)
        {
            //        ui->zxyCheckBox->setChecked(true);
            group.seq->setText(tr("Z->X->Y"));
        }
        else if(seqL == 2)
        {
            //            ui->yxzCheckBox->setChecked(true);
            group.seq->setText(tr("Y->X->Z"));
        }
        else if(seqL == 3)
        {
            //            ui->yzxCheckBox->setChecked(true);
            group.seq->setText(tr("Y->Z->X"));
        }
        //        ui->xRPLatticeLineEdit->SetThisIntToThisText(stackParams.at(ICMold::X_Array));
        group.xNum->setText(QString::number(stackParams.at(ICMold::X_Array)));
        //        ui->xRPCheckBox->setChecked(seqH & 32);
        group.xPosition->setText(seqH & 32 ? tr("RP") : tr("P"));
        //        ui->xRPStepLineEdit->SetThisIntToThisText(stackParams.at(ICMold::X_Gap));
        group.xStep->setText(QString::number(stackParams.at(ICMold::X_Gap)));
        //        ui->yRPLatticeLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Y_Array));
        group.yNum->setText(QString::number(stackParams.at(ICMold::Y_Array)));
        //                ui->yRPCheckBox->setChecked(seqH & 64);
        group.yPosition->setText(seqH & 64 ? tr("RP") : tr("P"));
        //        ui->yRPStepLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Y_Gap));
        group.yStep->setText(QString::number(stackParams.at(ICMold::Y_Gap)));
        //        ui->zRPLatticeLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Z_Array));
        group.zNum->setText(QString::number(stackParams.at(ICMold::Z_Array)));
        //        ui->zRPCheckBox->setChecked(seqH & 128);
        group.zPosition->setText(seqH & 128 ? tr("RP") : tr("P"));
        //        ui->zRPStepLineEdit->SetThisIntToThisText(stackParams.at(ICMold::Z_Gap));
        group.zStep->setText(QString::number(stackParams.at(ICMold::Z_Gap)));
    }
}

QList<ICMoldItem> ICStackEditor::CreateCommandImpl() const
{
    QList<ICMoldItem> ret;
    ICMoldItem item;
    for(int i = 0; i != stackGroups.size(); ++i)
    {
        if(stackGroups.at(i).check->isChecked())
        {
            item.SetClip(ICMold::ACTLAYOUTON);
            item.SetDVal(0);
            item.SetSVal(i);
            ret.append(item);
        }
    }
    return ret;
}
