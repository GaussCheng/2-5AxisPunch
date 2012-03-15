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

#include <QDebug>

MoldInformation * MoldInformation::instance_ = NULL;

MoldInformation::MoldInformation(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MoldInformation),
    recordFilePath_("./records/")
{
    ui->setupUi(this);

    UpdateInformationTable();
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
        item.SetAction(ICMold::ACTPOSEVERT);
        item.SetNum(0);
        items.append(item);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisX1, ICMold::GX, ICMold::ACTMAINBACKWARD);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisY1, ICMold::GY, ICMold::ACTMAINUP);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisZ, ICMold::GZ, -1);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisX2, ICMold::GP, ICMold::ACTVICEBACKWARD);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisY2, ICMold::GQ, ICMold::ACTVICEUP);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisA, ICMold::GA, -1);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisB, ICMold::GB, -1);
        CreateFileHelper_(items, ICVirtualHost::ICAxis_AxisC, ICMold::GC, -1);
        item.SetAction(ICMold::ACT_WaitMoldOpened);
        item.SetSVal(1);
        item.SetNum(1);
        items.append(item);
        item.SetAction(ICMold::ACTEND);
        item.SetNum(2);
        items.append(item);
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
        newFile.close();
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
    QTableWidgetItem * fileNameItem = new QTableWidgetItem(fileName);
    QTableWidgetItem * createDateTimeItem = new QTableWidgetItem(dateTime);
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
    //    QFile::copy(originConfigFilePath, targetConfigFilePath);
    if(QFile::copy(originFilePathName, targetFilePathName))
    {
        if(QFile::copy(originConfigFilePath, targetConfigFilePath))
        {
            QMessageBox::information(this, tr("Success"),
                                     tr("Copy file success!"),
                                     QMessageBox::Ok);
            return true;
        }
        QFile::remove(targetFilePathName);
    }
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
    configFile += "fnc";
    if(QFile::exists(filePathName))
    {
        QFile::remove(filePathName);
        QFile::remove(configFile);
        //        QFile::remove(ICSettingConfig::ConfigPath() + fileName);
//        QMessageBox::information(this, tr("Success"),
//                                 tr("File deleted success!"),
//                                 QMessageBox::Ok);
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
    fileInfoList_ = recordDir.entryInfoList(QDir::Files);

    QFileInfoList userProgramList;
    qDebug()<<"start1";
    foreach(const QFileInfo &fileInfo, fileInfoList_)
    {
        if(fileInfo.fileName().right(3) != "fnc")
        {
            userProgramList.append(fileInfo);
        }
    }
    qDebug()<<"end1";

    foreach(const QFileInfo &fileiInfo, userProgramList)
    {
        AddNewInTableWidget(fileiInfo.fileName(), fileiInfo.created().toString("yyyy-MM-dd"));
    }
    qDebug()<<"end2";
    ui->informationTableWidget->resizeColumnsToContents();
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
        AddNewInTableWidget(fileName, QDateTime::currentDateTime().toString("yyyy-MM-dd"));
        //        recordNames_->RecurdFileInfoListChanged(ui->destinationFileLineEdit->text(), ICRecordNames::ADDNEW);
        emit NewFileCreated(fileName);
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
        AddNewInTableWidget(fileName, QDateTime::currentDateTime().toString("yyyy-MM-dd"));
        //        recordNames_->RecurdFileInfoListChanged(ui->destinationFileLineEdit->text(), ICRecordNames::ARCHIVES_COPY);
        emit NewFileCreated(fileName);
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
        QString filePathName = "./records/" + moldName;
        if(QFile::exists(filePathName))
        {
            ICMold::CurrentMold()->ReadMoldFile(filePathName);
            ICVirtualHost::GlobalVirtualHost()->ReConfigure();
            qDebug("after emit updatehostparam");
            //        UpdateHostParam();

            ICParametersSave::Instance()->SetMoldName(moldName);
            ICProgramHeadFrame::Instance()->SetCurrentMoldName(moldName);
            QMessageBox::information(this, tr("Tips"), tr("Load Mold Successful!"));
        }
        qDebug("after load");
    }
}

void MoldInformation::on_deleteToolButton_clicked()
{
    if(ui->sourceFileNameLabel->text().isEmpty())
    {
        return;
    }

    if(ICParametersSave::Instance()->MoldName(QString()) == ui->sourceFileNameLabel->text())
    {
        QMessageBox::warning(this, tr("warning"),
                             tr("The mold file ") +
                             ui->sourceFileNameLabel->text() +
                             tr(" is being used"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    int ret = QMessageBox::warning(this, tr("warning"),
                                   tr("Are you sure to delete files ") +
                                   ui->sourceFileNameLabel->text(),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);
    if(ret != QMessageBox::Ok)
    {
        return;
    }

    if(DeleteSourceFile(ui->sourceFileNameLabel->text()))
    {
        ui->informationTableWidget->removeRow(ui->informationTableWidget->currentRow());
        emit DeleteFile(ui->sourceFileNameLabel->text());
        ui->sourceFileNameLabel->clear();
        if(ui->informationTableWidget->rowCount() != 0)
        {
            ui->sourceFileNameLabel->setText(
                        ui->informationTableWidget->item(ui->informationTableWidget->currentRow(), 0)->text());

        }
    }
}

void MoldInformation::on_informationTableWidget_clicked(QModelIndex index)
{
    QString fileName =  ui->informationTableWidget->item(index.row(), 0)->text();
    ui->sourceFileNameLabel->setText(fileName);
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
