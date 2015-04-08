#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>

#include "moldinformation.h"
#include "ui_moldinformation.h"
#include "icparameterssave.h"
#include "icprogramheadframe.h"
#include "icvirtualhost.h"
#include "config.h"
#include "ictipswidget.h"
#include "icbackuputility.h"
#include "icprogramformatchecker.h"
#include "icconfigformatchecker.h"
#include "icmacrosubroutine.h"
#include <QFileDialog>


#include <QDebug>

MoldInformation * MoldInformation::instance_ = NULL;

MoldInformation::MoldInformation(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MoldInformation),
    recordFilePath_("./records/")
{
    ui->setupUi(this);
    ui->buttonGroup->setId(ui->exportCheckBox,0);
    ui->buttonGroup->setId(ui->importCheckBox,1);
    QFile file("./sysconfig/StandPrograms");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString tmp;
        while(!(in>>tmp).atEnd())
        {
            standPrograms_.append(tmp);
        }
        file.close();
    }
    UpdateInformationTable();
    ui->informationTableWidget->setColumnWidth(1, 200);
    ui->informationTableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

    QList<QAbstractButton*> buttons = ui->buttonGroup->buttons();
    for(int i = 0; i != buttons.size(); ++i)
    {
        connect(buttons.at(i),
                SIGNAL(clicked()),
                this,
                SLOT(switchPushButton()));
    }
}

MoldInformation::~MoldInformation()
{
    delete ui;
}

void MoldInformation::changeEvent(QEvent *e)
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

void MoldInformation::showEvent(QShowEvent *e)
{
    //    UpdateInformationTable();
    QWidget::showEvent(e);
}

bool MoldInformation::CreateNewSourceFile(const QString & fileName)
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, tr("warning"),
                             tr("New file name is empty,\n"
                                "Please input the file name."),
                             QMessageBox::Ok);
        return false;
    }

    QDir recordFileDir(recordFilePath_);
    if(!recordFileDir.exists())
    {
        recordFileDir.cdUp();
        qDebug() << recordFileDir.mkpath(recordFilePath_);
    }

    QString filePathName = recordFilePath_ + fileName;
    QString fileNameNoExtent = fileName;
    fileNameNoExtent.chop(3);
    QFile newFile(filePathName);
    if(!newFile.exists())
    {
        if(!newFile.open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(this, tr("Warnning"),tr("This name can't not be accept!"));
            return false;
        }
#ifdef HC_8AXIS
        QList<ICMoldItem> items;
        ICMoldItem item;

        if(ICVirtualHost::GlobalVirtualHost()->AxisDefine(ICVirtualHost::ICAxis_AxisC) != ICVirtualHost::ICAxisDefine_Servo)
        {
            //            item.SetAction(ICMold::ACTPOSEVERT);
            //            item.SetNum(0);
            //            items.append(item);
        }
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisX1, ICMold::GX, -1);
        //        items[items.size() - 1].SetNum(items.size() - 1);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisY1, ICMold::GY, -1);
        //        items[items.size() - 1].SetNum(items.size() - 1);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisZ, ICMold::GZ, -1);
        //        items[items.size() - 1].SetNum(items.size() - 1);
        //        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisX2, ICMold::GP, -1);
        //        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisY2, ICMold::GQ, -1);
        //        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisA, ICMold::GA, -1);
        //        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisB, ICMold::GB, -1);
        //        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisC, ICMold::GC, -1);
        //        item.SetAction(ICMold::ACT_WaitMoldOpened);
        //        item.SetSVal(1);
        //        item.SetNum(1);
        //        items.append(item);
        item.SetAction(ICMold::ACTEND);
        //        item.SetNum(items.size() - 1);
        items.append(item);
        for(int i = 0; i != items.size(); ++i)
        {
            items[i].SetNum(i);
        }
        ICMold::MoldReSum(items);
        QByteArray toWrite;
        for(int i = 0; i != items.size(); ++i)
        {
            toWrite += items.at(i).ToString() + "\n";
        }
        newFile.write(toWrite);
        //        int axisDefine = ICVirtualHost::GlobalVirtualHost()->HostStatus(ICVirtualHost::SYS_Config_Arm).toInt();

        //        newFile.write("0 0 255 1 0 0 0 80 0 81\n1 1 255 2 0 0 0 80 0 84\n2 2 255 3 0 0 0 80 0 87\n3 3 255 4 0 0 0 80 0 90\n4 4 255 5 0 0 0 80 0 93\n5 5 255 6 0 0 0 80 0 96\n6 6 255 7 0 0 0 80 0 99\n7 7 255 0 0 0 0 80 0 94\n8 8 255 17 0 0 0 0 0 33\n9 8 255 14 0 0 0 0 0 31\n10 8 255 13 0 0 0 0 0 31\n11 9 255 29 0 0 0 1 0 50\n12 10 255 32 0 0 0 0 5 59");
#else
        newFile.write("0 0 255 1 0 0 0 80 0 81\n1 0 255 2 0 0 0 80 0 83\n2 0 255 3 0 0 0 80 0 85\n3 0 255 17 0 0 0 0 0 20\n4 0 255 14 0 0 0 0 0 18\n5 0 255 13 0 0 0 0 0 18\n6 1 255 29 0 0 0 1 0 37\n7 2 255 32 0 0 0 0 5 46");
#endif
        QFile::copy(recordFilePath_ + "/Base.fnc", recordFilePath_ + "/" + fileNameNoExtent + "fnc");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "sub");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "reserve1");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "reserve2");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "reserve3");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "reserve4");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "reserve5");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "reserve6");
        QFile::copy(recordFilePath_ + "/Base.sub", recordFilePath_ + "/" + fileNameNoExtent + "reserve7");
        newFile.close();
        ::system("sync");
        QMessageBox::warning(this, tr("Success"),
                             tr("New file success."),
                             QMessageBox::Ok);
        return true;
    }
    else
    {
        QMessageBox::warning(this, tr("The file has been existing"),
                             tr("File already exists,\n"
                                "Please change a new name."),
                             QMessageBox::Ok);
    }
    return false;
}

void MoldInformation::AddNewInTableWidget(const QString & fileName, const QString & dateTime)
{
    QTableWidgetItem * fileNameItem = new QTableWidgetItem(fileName.left(fileName.size() - 4));
    QTableWidgetItem * createDateTimeItem = new QTableWidgetItem(dateTime);
    fileNameItem->setCheckState(Qt::Unchecked);
    ui->informationTableWidget->insertRow(ui->informationTableWidget->rowCount());
    ui->informationTableWidget->setItem(ui->informationTableWidget->rowCount() - 1,0,fileNameItem);
    ui->informationTableWidget->setItem(ui->informationTableWidget->rowCount() - 1,1,createDateTimeItem);
}
bool MoldInformation::CopySourceFile(const QString & originFileName, const QString & targetFileName)
{
    if(originFileName.isEmpty())
    {
        QMessageBox::warning(this, tr("warning"),
                             tr("Source file name is empty,\n"
                                "Please choose an existing file."),
                             QMessageBox::Ok);
        return false;
    }
    if(targetFileName.isEmpty())
    {
        QMessageBox::warning(this, tr("warning"),
                             tr("New file name is empty,\n"
                                "Please write the destination file name."),
                             QMessageBox::Ok);
        return false;
    }

    QString originFilePathName = recordFilePath_ + originFileName;
    QString targetFilePathName = recordFilePath_ + targetFileName;

    QString originConfigFilePath = originFilePathName;
    originConfigFilePath.chop(3);
    originConfigFilePath += "fnc";
    QString targetConfigFilePath = targetFilePathName;
    targetConfigFilePath.chop(3);
    targetConfigFilePath += "fnc";
    QString originSubFilePath = originFilePathName;
    originSubFilePath.chop(3);
    originSubFilePath += "sub";
    QString targetSubFilePath = targetFilePathName;
    targetSubFilePath.chop(3);
    targetSubFilePath += "sub";
    QString originResvFilePath = originFilePathName;
    originResvFilePath.chop(3);
    originResvFilePath += "reserve";
    QString targetResvFilePath = targetFilePathName;
    targetResvFilePath.chop(3);
    targetResvFilePath += "reserve";
    //    QFile::copy(originConfigFilePath, targetConfigFilePath);
    if(QFile::copy(originFilePathName, targetFilePathName))
    {
        if(QFile::copy(originConfigFilePath, targetConfigFilePath))
        {
            if(QFile::copy(originSubFilePath, targetSubFilePath))
            {
                bool isOk = true;
                for(int i = 1; i != 8; ++i)
                {
                    isOk = isOk && QFile::copy(originResvFilePath+ QString::number(i), targetResvFilePath + QString::number(i));
//                    if(QFile::copy(originResvFilePath, targetResvFilePath))
//                    {
//                        QMessageBox::information(this, tr("Success"),
//                                                 tr("Copy file success!"),
//                                                 QMessageBox::Ok);
//                        return true;
//                    }
                }
                if(isOk) return true;
                system(QString("rm %1*").arg(targetResvFilePath).toLatin1());
                QFile::remove(targetSubFilePath);
            }
            QFile::remove(targetConfigFilePath);
        }
        QFile::remove(targetFilePathName);
    }
    ::system("sync");
    QMessageBox::information(this, tr("warning"),
                             tr("Destination file already exists!\n"
                                "Please try a new name"),
                             QMessageBox::Ok);
    return false;
}

bool MoldInformation::DeleteSourceFile(const QString & fileName)
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, tr("warning"),
                             tr("Source file name is empty,\n"
                                "Please choose an existing file."),
                             QMessageBox::Ok);
        return false;
    }

    QString filePathName = recordFilePath_ + fileName;
    QString configFile = filePathName;
    configFile.chop(3);
    QString subFile = configFile + "sub";
    QString resvFile = configFile + "reserve";
    configFile += "fnc";
    if(QFile::exists(filePathName))
    {
        QFile::remove(filePathName);
        QFile::remove(configFile);
        QFile::remove(subFile);
//        QFile::remove(resvFile);
        QString toRm = QString("rm %1*").arg(resvFile);
//        qDebug() << toRm.toUtf8();
        system(toRm.toUtf8()); //中文采用utf-8编码
        //        QFile::remove(ICSettingConfig::ConfigPath() + fileName);
        //        QMessageBox::information(this, tr("Success"),
        //                                 tr("File deleted success!"),
        //                                 QMessageBox::Ok);
        system("sync");
        return true;
    }
    else
    {
        QMessageBox::warning(this, tr("warning"),
                             tr("File does not exist!"),
                             QMessageBox::Ok);
    }
    return false;
}

void MoldInformation::UpdateInformationTable()
{
    ui->informationTableWidget->clearContents();
    ui->informationTableWidget->setRowCount(0);

    fileInfoList_.clear();
    QDir recordDir(recordFilePath_);
    fileInfoList_ = recordDir.entryInfoList(QStringList()<<"*.act" ,QDir::Files);

    QFileInfoList userProgramList;
    qDebug()<<"start1";
    QFileInfo tmp;
//    QString moldName;
    foreach(tmp, fileInfoList_)
    {
//        moldName = tmp.fileName().left(tmp.fileName().indexOf('.'));
        if(IsStandProgram(tmp.fileName()))
        {
            userProgramList.prepend(tmp);
        }
        else if(tmp.fileName().right(4) != ".fnc")
        {
            userProgramList.append(tmp);
        }
    }
    qDebug()<<"end1";

    foreach(tmp, userProgramList)
    {
        AddNewInTableWidget(tmp.fileName(), tmp.lastModified().toString("yyyy/MM/dd hh:mm:ss"));
    }
    qDebug()<<"end2";
    //    ui->informationTableWidget->resizeColumnsToContents();
}

void MoldInformation::on_newToolButton_clicked()
{
    if(ui->destinationFileLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("file name is empty"));
        return;
    }
    QString fileName = ui->destinationFileLineEdit->text() + ".act";
    if(CreateNewSourceFile(fileName))
    {
        AddNewInTableWidget(fileName, QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
        //        recordNames_->RecurdFileInfoListChanged(ui->destinationFileLineEdit->text(), ICRecordNames::ADDNEW);
        //        emit NewFileCreated(fileName);
        ui->destinationFileLineEdit->clear();
    }
}

void MoldInformation::on_copyToolButton_clicked()
{
    if(ui->destinationFileLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("file name is empty"));
        return;
    }
    QString fileName = ui->destinationFileLineEdit->text() + ".act";
    if(CopySourceFile(ui->sourceFileNameLabel->text(), fileName))
    {
        AddNewInTableWidget(fileName, QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
        //        recordNames_->RecurdFileInfoListChanged(ui->destinationFileLineEdit->text(), ICRecordNames::ARCHIVES_COPY);
        //        emit NewFileCreated(fileName);
        ui->destinationFileLineEdit->clear();
    }
}

void MoldInformation::on_loadToolButton_clicked()
{
    int currentRow = ui->informationTableWidget->currentRow();
    if(currentRow != -1)
    {
        qDebug("before Load");
        QString moldName = ui->informationTableWidget->item(currentRow, 0)->text();
        QString subName = moldName + ".sub";
        QString resvName = moldName + ".reserve";
        moldName += ".act";
        if(ICParametersSave::Instance()->MoldName(QString()) == moldName)
        {
            QMessageBox::information(this, tr("Tips"), tr("On the Current mold already!"));
            return;
        }

        QString filePathName = "./records/" + moldName;
        if(QFile::exists(filePathName))
        {
            emit ReadyToLoad(moldName);
            if(!ICMold::CurrentMold()->ReadMoldFile(filePathName))
            {
                QMessageBox::critical(this, tr("critical"), tr("Read mold or mold para fail! Please change other mold!"));
                //                ICMold::CurrentMold()->ReadMoldFile(
                return;
            }
            if(!QFile::exists(QString("./records/%1").arg(subName)) ||
                    !QFile::exists(QString("./records/%1%2").arg(resvName).arg(1)) ||
                    !QFile::exists(QString("./records/%1%2").arg(resvName).arg(2)) ||
                    !QFile::exists(QString("./records/%1%2").arg(resvName).arg(3)) ||
                    !QFile::exists(QString("./records/%1%2").arg(resvName).arg(4)) ||
                    !QFile::exists(QString("./records/%1%2").arg(resvName).arg(5)) ||
                    !QFile::exists(QString("./records/%1%2").arg(resvName).arg(6)) ||
                    !QFile::exists(QString("./records/%1%2").arg(resvName).arg(7)))
            {
                QMessageBox::critical(this, tr("critical"), tr("Mold is break, please remove it!"));
                return;
            }
            system(QString("cp ./records/%1 ./subs/sub7.prg -f").arg(subName).toLatin1());
            system(QString("cp ./records/%1%2 ./subs/sub0.prg -f").arg(resvName).arg(1).toLatin1());
            system(QString("cp ./records/%1%2 ./subs/sub1.prg -f").arg(resvName).arg(2).toLatin1());
            system(QString("cp ./records/%1%2 ./subs/sub2.prg -f").arg(resvName).arg(3).toLatin1());
            system(QString("cp ./records/%1%2 ./subs/sub3.prg -f").arg(resvName).arg(4).toLatin1());
            system(QString("cp ./records/%1%2 ./subs/sub4.prg -f").arg(resvName).arg(5).toLatin1());
            system(QString("cp ./records/%1%2 ./subs/sub5.prg -f").arg(resvName).arg(6).toLatin1());
            system(QString("cp ./records/%1%2 ./subs/sub6.prg -f").arg(resvName).arg(7).toLatin1());
            ICMacroSubroutine::Instance()->ReadMacroSubroutieFiles("./subs");
            ICTipsWidget tipsWidget(tr("Loading..."));
            tipsWidget.show();qApp->processEvents();
            ICVirtualHost::GlobalVirtualHost()->ReConfigure();
            qDebug("after emit updatehostparam");
            //        UpdateHostParam();

            ICParametersSave::Instance()->SetMoldName(moldName);
            ICProgramHeadFrame::Instance()->SetCurrentMoldName(moldName);
            emit MoldChanged(moldName);
            //            QMessageBox::information(this, tr("Tips"), tr("Load Mold Successful!"));
        }
        ICVirtualHost::GlobalVirtualHost()->SetFixtureCheck(true);
        qDebug("after load");
        system("sync");
    }
}

void MoldInformation::on_deleteToolButton_clicked()
{
    QString selectedItem;
    bool flag;
    flag = false;
    QList<int> selectedItemNumberList ;
    QStringList selectedItemStringList ;
    int rows = ui->informationTableWidget->rowCount();


    for(int i = 0 ; i < rows ; ++i)
    {
        if(ui->informationTableWidget->item(i,0)->checkState() == Qt::Unchecked)
        {
            continue;
        }
        selectedItem = ui->informationTableWidget->item(i,0)->text()+".act";

        if(ICParametersSave::Instance()->MoldName(QString()) == selectedItem)
        {
            QMessageBox::warning(this, tr("warning"),
                                 tr("The mold file ") +
                                 selectedItem +
                                 tr(" is being used"),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            flag = true;

            continue;
        }

        if(IsStandProgram(selectedItem))
        {
            QMessageBox::warning(this, tr("warning"),
                                 tr("Stand programs can not be delete!"));
            flag = true;
            continue;
        }
        selectedItemStringList << selectedItem;
        selectedItemNumberList << i;
    }
    //同导出，当没有勾选模号时，删除当前行的模号
    QString str = ui->sourceFileNameLabel->text();
    if(!flag && !str.isEmpty() && (selectedItemStringList.size() == 0))

        //        if(!str.isEmpty() && (selectedItemStringList.size() == 0) && \
        //                (str != ICParametersSave::Instance()->MoldName("")) && \
        //                !IsStandProgram(str))
    {
        str.chop(4);
        if((str + ".act") == ICParametersSave::Instance()->MoldName(""))
        {
            QMessageBox::warning(this, tr("warning"),
                                 tr("The mold file ") +
                                 str +
                                 tr(" is being used"),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return;
        }
        if(IsStandProgram(str + ".act"))
        {
            QMessageBox::warning(this, tr("warning"),
                                 tr("Stand programs can not be delete!"));
            return;
        }

        selectedItemStringList << ui->informationTableWidget->item(ui->informationTableWidget->currentRow(),0)->text() + ".act" ;
        selectedItemNumberList << ui->informationTableWidget->currentRow();
    }
    if(selectedItemStringList.size() != 0)
    {
        int ret = QMessageBox::warning(this, tr("warning"),
                                       tr("Are you sure to delete the selected files?"),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);
        if(ret != QMessageBox::Ok)
        {
            return ;
        }
    }

    for(int i = 0 ; i < selectedItemNumberList.size() ; ++i)
    {
        if(DeleteSourceFile(selectedItemStringList[i]))
        {
            if(i == 0)
                ui->informationTableWidget->removeRow(selectedItemNumberList[i]);
            else
                ui->informationTableWidget->removeRow(selectedItemNumberList[i] - i);

            //            ui->sourceFileNameLabel->clear();
            //            if(ui->informationTableWidget->rowCount() != 0)
            //            {
            //                ui->sourceFileNameLabel->setText(
            //                            ui->informationTableWidget->item(0,0)->text() + ".act");
            //            }
        }
    }


}

void MoldInformation::CreateFileHelper_(QList<ICMoldItem> &items, int axis, int servo, int pneumatic)
{
    ICMoldItem item;
    item.SetSVal(80);
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    if(host->AxisDefine(static_cast<ICVirtualHost::ICAxis>(axis)) == ICVirtualHost::ICAxisDefine_Pneumatic)
    {
        if(pneumatic > 0)
        {
            item.SetAction(pneumatic);
            items.append(item);
        }
    }
    else if(host->AxisDefine(static_cast<ICVirtualHost::ICAxis>(axis)) == ICVirtualHost::ICAxisDefine_Servo)
    {
        item.SetAction(servo);
        items.append(item);
    }
}

void MoldInformation::on_informationTableWidget_itemSelectionChanged()
{
    QList<QTableWidgetItem*> items = ui->informationTableWidget->selectedItems();
    if(items.isEmpty())
    {
        return;
    }
    int row = ui->informationTableWidget->row(items.at(0));
    //    ui->informationTableWidget->selectRow(row);
    //    ui->informationTableWidget->setCurrentCell(row, 0);
    ui->informationTableWidget->setCurrentItem(ui->informationTableWidget->item(row, 0));
    ui->sourceFileNameLabel->setText(ui->informationTableWidget->currentItem()->text() + ".act");
    //    on_informationTableWidget_itemEntered(items[0]);
}
void MoldInformation::on_importToolButton_clicked()
{
    selectedImportItemName_.clear();
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    if(ui->informationTableWidget->rowCount() == 0)
    {
        RefreshFileList();
        return;
    }
    QDir dir(getFileDir_ + "/HC5ABackup/records");
#else

    QDir dir("/mnt/udisk/HC5ABackup/records");
#endif
    QDir src_dir("./records/");
    if(!dir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        //        ui->exportCheckBox->setChecked(true);
        return;
    }

    QStringList acts = dir.entryList(QStringList()<<"*.act");
    QStringList fncs = dir.entryList(QStringList()<<"*.fnc");
    QStringList actsubs = dir.entryList(QStringList()<<"*.sub");
    QStringList reserves = dir.entryList(QStringList()<<"*.reserve*");
    acts_ = src_dir.entryList(QStringList()<<"*.act");


    //    if(fncs.contains("Base.fnc"))
    //    {
    //        fncs.removeOne("Base.fnc");
    //    }
    //    if(acts.size() != fncs.size())
    //    {
    //        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
    //        return;
    //    }
    //    for(int i = 0; i != fncs.size(); ++i)
    //    {
    //        fncs[i] = fncs.at(i).left(fncs.at(i).size() - 4);
    //    }
    //    for(int i = 0; i != acts.size(); ++i)
    //    {
    //        if(!fncs.contains(acts.at(i).left(acts.at(i).size() - 4)))
    //        {
    //            QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
    //            return;
    //        }
    //    }

    int rows_ = ui->informationTableWidget->rowCount();
    bool flagItem = TRUE ;
    bool flagItem_ = TRUE ;
    bool flag = false ;
    for(int i = 0 ; i < rows_ ; ++i)
    {
        if(ui->informationTableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            QString item_text = ui->informationTableWidget->item(i,0)->text();

            if(acts_.contains(item_text+".act"))
            {
                if(ICParametersSave::Instance()->MoldName(QString()) == item_text + ".act")
                {
                    QMessageBox::warning(this, tr("warning"),
                                         tr("The mold file ") +
                                         item_text +
                                         tr(" is being used"),
                                         QMessageBox::Ok,
                                         QMessageBox::Ok);
                    flag = true;
                    continue;
                }

                if(QMessageBox::question(this,tr("tips"),tr( "%1 is exist,replace it?").arg(item_text),
                                         QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Cancel)
                {
                    flag = true;
                    continue;
                }
            }
            selectedImportItemName_.append(item_text + ".act");
            selectedImportItemName_.append(item_text + ".fnc");
            selectedImportItemName_.append(item_text + ".sub");
            selectedImportItemName_.append(item_text + ".reserve1");
            selectedImportItemName_.append(item_text + ".reserve2");
            selectedImportItemName_.append(item_text + ".reserve3");
            selectedImportItemName_.append(item_text + ".reserve4");
            selectedImportItemName_.append(item_text + ".reserve5");
            selectedImportItemName_.append(item_text + ".reserve6");
            selectedImportItemName_.append(item_text + ".reserve7");
            flagItem = FALSE ;
        }
    }

    //如果没有勾选模号，那么可以导入选中的当前行的模号
    QString str = ui->sourceFileNameLabel->text();
    //    if((selectedImportItemName_.size() == 0) && (!acts_.contains(str + ".act")))
    if(!flag && !str.isEmpty() && (selectedImportItemName_.size() == 0))
    {
        str.chop(4);
        if(acts_.contains(str + ".act"))
        {
            if(QMessageBox::question(this,tr("tips"),tr( "%1 is exist,replace it?").arg(str),
                                     QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Cancel)
            {
                return;
            }
            if(ICParametersSave::Instance()->MoldName(QString()) == str + ".act")
            {
                QMessageBox::warning(this, tr("warning"),
                                     tr("The mold file ") +
                                     str +
                                     tr(" is being used"),
                                     QMessageBox::Ok,
                                     QMessageBox::Ok);
                return;
            }
        }
        selectedImportItemName_.append(str + ".act");
        selectedImportItemName_.append(str + ".fnc");
        selectedImportItemName_.append(str + ".sub");
        selectedImportItemName_.append(str + ".reserve1");
        selectedImportItemName_.append(str + ".reserve2");
        selectedImportItemName_.append(str + ".reserve3");
        selectedImportItemName_.append(str + ".reserve4");
        selectedImportItemName_.append(str + ".reserve5");
        selectedImportItemName_.append(str + ".reserve6");
        selectedImportItemName_.append(str + ".reserve7");
        flagItem_ = FALSE;
    }

    QFile file;
    QString actContent;
    ICProgramFormatChecker programChecker;
    ICConfigFormatChecker configFormatChecker;
    for(int i = 0; i != selectedImportItemName_.size(); ++i)
    {
        file.setFileName(dir.absoluteFilePath(selectedImportItemName_.at(i)));
        actContent.clear();
        file.open(QFile::ReadOnly | QFile::Text);
        actContent = file.readAll();
        file.close();
        if(selectedImportItemName_.at(i).endsWith(".act"))
        {
            if(!programChecker.Check(actContent))
            {
                QMessageBox::warning(this, tr("Warnning"), QString(tr("%1 Wrong program format!")).arg(selectedImportItemName_.at(i)));
                return;
            }
        }
        else if(selectedImportItemName_.at(i).endsWith(".fnc"))
        {
            if(!configFormatChecker.CheckRowCount(actContent, ICMold::MoldParamCount +  ICMold::StackParamCount * 4 + 1,ICDataFormatChecker::kCompareEqual))
            {
                QMessageBox::warning(this, tr("Warnning"), QString(tr("%1 Wrong config format!!!")).arg(selectedImportItemName_.at(i)));
                return;
            }
            if(!configFormatChecker.Check(actContent))
            {
                QMessageBox::warning(this, tr("Warnning"), QString(tr("%1 Wrong config format!!!")).arg(selectedImportItemName_.at(i)));
                return;
            }
        }
        else if(selectedImportItemName_.at(i).endsWith(".sub") ||
                selectedImportItemName_.at(i).endsWith(".reserve1") ||
                selectedImportItemName_.at(i).endsWith(".reserve2") ||
                selectedImportItemName_.at(i).endsWith(".reserve3") ||
                selectedImportItemName_.at(i).endsWith(".reserve4") ||
                selectedImportItemName_.at(i).endsWith(".reserve5") ||
                selectedImportItemName_.at(i).endsWith(".reserve6") ||
                selectedImportItemName_.at(i).endsWith(".reserve7"))
        {
            if(!programChecker.Check(actContent))
            {
                QMessageBox::warning(this, tr("Warnning"), QString(tr("%1 Wrong program format!")).arg(selectedImportItemName_.at(i)));
                return;
            }
        }
    }

    ICTipsWidget tipsWidget(tr("Restoring, please wait..."));
    if(!flagItem || !flagItem_)
    {
        tipsWidget.show();
        qApp->processEvents();
    }

    if(selectedImportItemName_.size() == 0)
    {
        return;
    }
    ICBackupUtility backupUtility;
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    bool ret = backupUtility.RestoreDir(getFileDir_ + "/HC5ABackup/records",
                                        "./records",
                                        selectedImportItemName_);
#else
    bool ret = backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/records",
                                        "./records",
                                        selectedImportItemName_);
#endif
    dir.cdUp();
    dir.cd("subs");
    QStringList subs = dir.entryList(selectedImportItemName_<<"sub[0-7]");
    for(int i = 0; i != subs.size(); ++i)
    {
        file.setFileName(dir.absoluteFilePath(subs.at(i)));
        actContent.clear();
        file.open(QFile::ReadOnly | QFile::Text);
        actContent = file.readAll();
        file.close();
        if(!programChecker.Check(actContent))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Wrong program format!!!"));
            return;
        }
    }

#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    if(ret)
    {
        ret = ret && backupUtility.RestoreDir(getFileDir_ +"/HC5ABackup/subs",
                                              "./subs",
                                              selectedImportItemName_<<"sub[0-7].prg");
    }
#else
    if(ret)
    {
        ret = ret && backupUtility.RestoreDir("/mnt/udisk/HC5ABackup/subs",
                                              "./subs",
                                              selectedImportItemName_<<"sub[0-7].prg");
    }
#endif
    if(!flagItem || !flagItem_)
    {
        ::system("sync");
        QMessageBox::information(this,tr("Information"), tr("Operation finished!"));
    }
}

void MoldInformation::on_exportToolButton_clicked()
{
    selectedExportItemName_.clear();

#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    QString getFileDir = QFileDialog::getExistingDirectory();
    QDir dir(getFileDir + "/HC5ABackup/records");
#else
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        return;
    }
    QDir dir("/mnt/udisk/HC5ABackup/records");
    if(!dir.exists())
    {
        system("mkdir -p /mnt/udisk/HC5ABackup/records");
    }
#endif
    acts_ = dir.entryList(QStringList()<<"*.act");
    int rows = ui->informationTableWidget->rowCount();
    bool flagItem = TRUE ;
    bool flagItem_ = TRUE ;
    bool flag = false;
    for(int i = 0 ; i < rows ; ++i)
    {
        if(ui->informationTableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            QString item_text = ui->informationTableWidget->item(i,0)->text();

            if(acts_.contains(item_text+".act"))
            {
                if(QMessageBox::question(this,tr("tips"),tr( "%1 is exist,replace it?").arg(item_text),
                                         QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Cancel)
                {
                    flag = true;
                    continue;
                }
            }
            selectedExportItemName_.append(item_text + ".act");
            selectedExportItemName_.append(item_text + ".fnc");
            selectedExportItemName_.append(item_text + ".sub");
            selectedExportItemName_.append(item_text + ".reserve1");
            selectedExportItemName_.append(item_text + ".reserve2");
            selectedExportItemName_.append(item_text + ".reserve3");
            selectedExportItemName_.append(item_text + ".reserve4");
            selectedExportItemName_.append(item_text + ".reserve5");
            selectedExportItemName_.append(item_text + ".reserve6");
            selectedExportItemName_.append(item_text + ".reserve7");
            flagItem = FALSE ;
        }
    }

    //如果没有勾选模号，那么可以导出选中的当前行的模号
    QString str = ui->sourceFileNameLabel->text();
    if(!flag && !str.isEmpty() && (selectedExportItemName_.size() == 0))
        //    if(selectedExportItemName_.size() == 0 && !acts_.contains(str + ".act"))
    {
        str.chop(4);
        if(acts_.contains(str + ".act"))
        {
            if(QMessageBox::question(this,tr("tips"),tr( "%1 is exist,replace it?").arg(str),
                                     QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Cancel)
            {
                return;
            }
        }
        selectedExportItemName_.append(str + ".act");
        selectedExportItemName_.append(str + ".fnc");
        selectedExportItemName_.append(str + ".sub");
        selectedExportItemName_.append(str + ".reserve1");
        selectedExportItemName_.append(str + ".reserve2");
        selectedExportItemName_.append(str + ".reserve3");
        selectedExportItemName_.append(str + ".reserve4");
        selectedExportItemName_.append(str + ".reserve5");
        selectedExportItemName_.append(str + ".reserve6");
        selectedExportItemName_.append(str + ".reserve7");
        flagItem_ = FALSE;
    }
    if(selectedExportItemName_.size() == 0)
    {
        return;
    }

    ICTipsWidget tipsWidget(tr("Backuping, please wait..."));
    if(!flagItem || !flagItem_)
    {
        tipsWidget.show();
        qApp->processEvents();
    }

    ICBackupUtility backupUtility;
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    bool ret = backupUtility.BackupDir("./records",
                                       getFileDir + "/HC5ABackup/records",
                                       selectedExportItemName_);
    ret = ret && backupUtility.BackupDir("./subs",
                                         getFileDir + "/HC5ABackup/subs",
                                         selectedExportItemName_<<"sub[0-7].prg");

#else

    bool ret = backupUtility.BackupDir("./records",
                                       "/mnt/udisk/HC5ABackup/records",
                                       selectedExportItemName_);
    ret = ret && backupUtility.BackupDir("./subs",
                                         "/mnt/udisk/HC5ABackup/subs",
                                         selectedExportItemName_<<"sub[0-7].prg");
#endif
    if(!flagItem || !flagItem_)
    {
//        system("umount /mnt/udisk");
        system("sync");
        QMessageBox::information(this,tr("Information"), tr("Operation finished!"));
    }

}


void MoldInformation::switchPushButton()
{
    switch (ui->buttonGroup->checkedId())
    {
    case 0:
        ui->newToolButton->setEnabled(true);
        ui->copyToolButton->setEnabled(true);
        ui->loadToolButton->setEnabled(true);
        ui->deleteToolButton->setEnabled(true);
        ui->exportToolButton->setEnabled(true);
        ui->importToolButton->setEnabled(false);
        UpdateInformationTable();
        ui->sourceFileNameLabel->clear();
        break;
    case 1:
        //        ui->newToolButton->setEnabled(false);
        //        ui->copyToolButton->setEnabled(false);
        //        ui->loadToolButton->setEnabled(false);
        //        ui->deleteToolButton->setEnabled(false);
        //        ui->exportToolButton->setEnabled(false);
        //        ui->importToolButton->setEnabled(true);
        //        ui->sourceFileNameLabel->clear();
        RefreshFileList();
        break;
    }
}

void MoldInformation::RefreshFileList()
{
#ifdef Q_WS_QWS
    if(!CheckIsUsbAttached())
    {
        QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
        ui->exportCheckBox->click();
        return;
    }
#endif
    ui->newToolButton->setEnabled(false);
    ui->copyToolButton->setEnabled(false);
    ui->loadToolButton->setEnabled(false);
    ui->deleteToolButton->setEnabled(false);
    ui->exportToolButton->setEnabled(false);
    ui->importToolButton->setEnabled(true);
    ui->sourceFileNameLabel->clear();

    ui->informationTableWidget->clearContents();
    ui->informationTableWidget->setRowCount(0);
    fileInfoList_.clear();
#if defined(Q_WS_WIN32) || defined(Q_WS_X11)
    getFileDir_ = QFileDialog::getExistingDirectory();
    QDir recordDir(getFileDir_ + "/HC5ABackup/records");
#else
    QDir recordDir("/mnt/udisk/HC5ABackup/records");

    if(!recordDir.exists())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is not exists!"));
        return;
    }
    ICTipsWidget tipsWidget(tr("Refresh File List, please wait..."));
    tipsWidget.show();
    qApp->processEvents();
#endif


    QStringList acts = recordDir.entryList(QStringList()<<"*.act");
    QStringList fncs = recordDir.entryList(QStringList()<<"*.fnc");
    if(fncs.contains("Base.fnc"))
    {
        fncs.removeOne("Base.fnc");
    }
    if(acts.size() != fncs.size())
    {
        QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
//        return;
    }
    for(int i = 0; i != fncs.size(); ++i)
    {
        fncs[i] = fncs.at(i).left(fncs.at(i).size() - 4);
    }
    for(int i = 0; i != acts.size(); ++i)
    {
        if(!fncs.contains(acts.at(i).left(acts.at(i).size() - 4)))
        {
            QMessageBox::warning(this, tr("Warnning"), tr("Backup files is incomplete!"));
//            return;
        }
    }

    fileInfoList_ = recordDir.entryInfoList(QStringList()<<"*.act", QDir::Files);

    QFileInfoList userProgramList;
    QFileInfo tmp;
    foreach(tmp, fileInfoList_)
    {
        if(IsStandProgram(tmp.fileName()))
        {
            userProgramList.prepend(tmp);
        }
        else if(tmp.fileName().right(4) != ".fnc")
        {
            userProgramList.append(tmp);
        }
    }
    foreach(tmp, userProgramList)
    {
        AddNewInTableWidget(tmp.fileName(), tmp.lastModified().toString("yyyy/MM/dd hh:mm:ss"));
    }


}

void MoldInformation::on_allToolButton_clicked()
{
    int rows_ = ui->informationTableWidget->rowCount();

    for(int i = 0 ; i < rows_ ; ++i)
    {
        ui->informationTableWidget->item(i,0)->setCheckState(Qt::Checked);
    }
}

void MoldInformation::on_InverseToolButton_clicked()
{
    int rows_ = ui->informationTableWidget->rowCount();

    for(int i = 0 ; i < rows_ ; ++i)
    {
        if(ui->informationTableWidget->item(i,0)->checkState() == Qt::Checked)
            ui->informationTableWidget->item(i,0)->setCheckState(Qt::Unchecked);
        else
            ui->informationTableWidget->item(i,0)->setCheckState(Qt::Checked);
    }
}

void MoldInformation::on_unselectToolButton_clicked()
{
    int rows_ = ui->informationTableWidget->rowCount();

    for(int i = 0 ; i < rows_ ; ++i)
    {
        ui->informationTableWidget->item(i,0)->setCheckState(Qt::Unchecked);
    }
}

bool MoldInformation::CheckIsUsbAttached() const
{
    QDir dir("/proc/scsi/usb-storage");
    if(!dir.exists())
    {
        return false;
    }
    if(dir.entryList(QStringList(), QDir::Files).isEmpty())
    {
        return false;
    }
    QFile file("/proc/mounts");
    if(!file.open(QFile::ReadOnly))
    {
        return false;
    }
    QString content = file.readAll();
    file.close();
    return content.contains(QRegExp("/dev/sd[a-z]*[1-9]*"));

}

void MoldInformation::on_destinationFileLineEdit_textChanged(const QString &arg1)
{
    if(arg1.contains(QRegExp("\\W")))
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Only accept number and word"));
        ui->destinationFileLineEdit->clear();
    }
}
