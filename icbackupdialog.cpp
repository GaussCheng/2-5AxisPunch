#include "icbackupdialog.h"
#include "ui_icbackupdialog.h"
#include <QKeyEvent>
#include <QMessageBox>
#include "ictipswidget.h"
#include "icutility.h"
#include "icbackuputility.h"

ICBackupDialog::ICBackupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICBackupDialog)
{
    ui->setupUi(this);
}

ICBackupDialog::~ICBackupDialog()
{
    delete ui;
}

void ICBackupDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void ICBackupDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
#ifdef Q_WS_QWS
    case Qt::Key_M://No
#else
    case Qt::Key_F4:
#endif
        this->reject();
        break;
#ifdef Q_WS_QWS
    case Qt::Key_H://Yes
#else
    case Qt::Key_F5:
#endif
    {
//        ::system("touch /home/root/recal && sync && reboot");if(!CheckIsUsbAttached())
        if(!ICUtility::IsUsbAttached())
        {
            QMessageBox::warning(this, tr("Warning"), tr("USB is not connected!"));
            return;
        }
        ICTipsWidget tipsWidget(tr("Backuping, please wait..."));
        tipsWidget.show();qApp->processEvents();
        ICBackupUtility backupUtility;
        bool ret = backupUtility.BackupDir("./sysconfig",
                                           "/mnt/udisk/HC5ABackup/sysconfig",
                                           QStringList()<<"param*.txt"<<"DistanceRotation");
        ret = ret && backupUtility.BackupDir("./sysconfig",
                                             "/mnt/udisk/HC5ABackup/sysconfig",
                                             QStringList()<<"system.txt");

        ret = ret && backupUtility.BackupDir("./records/",
                                             "/mnt/udisk/HC5ABackup/records/",
                                             QStringList()<<"*.act"<<"*.fnc"<<"*.sub"<<"*.reserve*");

        ret = ret && backupUtility.BackupDir("./subs",
                                             "/mnt/udisk/HC5ABackup/subs",
                                             QStringList()<<"sub[0-7].prg");
        if(ret)
        {
            QMessageBox::information(this, tr("Backup"), tr("Backup all configs successfully!"));
        }
        else
        {
            QMessageBox::information(this, tr("Backup"), tr("Backup all configs fail!"));
        }

        this->accept();
    }
        break;
    default:
        break;
    }
}
