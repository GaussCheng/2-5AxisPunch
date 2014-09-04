#include "icsettingsframe.h"
#include "ui_icsettingsframe.h"
#include "icupdatesystempage.h"
#include "icmachinestructpage.h"
#include "icmachineconfigpage.h"
#include "ichcproductsettingframe.h"
#include "ichcsystemsettingsframe.h"
#include <QProxyStyle>
#include <QPainter>
#include <QDateTime>
#include "icutility.h"
static void BuildShiftMap(int beg, int* map)
{
    int index = 0;
    for(int i = beg; i < 10; ++i)
    {
        map[index++] = i;
    }
    for(int i = 0; i < beg; ++i)
    {
        map[index++] = i;
    }
}

int Register(const QString& code, const QString& machineCode)
{

    int i,j;
    int pMap[10] = {0};
    int sortMap[20] = {0};
    char sortRet[19] = {0};
    long beg;
    quint8 buffer[20];

    sortMap[0] = 1;
    sortMap[1] = 2;
    sortMap[2] = 3;
    sortMap[3] = 5;
    sortMap[4] = 7;
    sortMap[5] = 11;
    sortMap[6] = 13;
    sortMap[7] = 0;
    sortMap[8] = 4;
    sortMap[9] = 6;
    sortMap[10] = 8;
    sortMap[11] = 10;
    sortMap[12] = 12;
    sortMap[13] = 14;
    sortMap[14] = 9;
    sortMap[15] = 15;
    sortMap[16] = 17;
    sortMap[17] = 16;
    sortMap[18] = 0;
    sortMap[19] = 0;

    if(code.size() != 20)
    {
        return -1;
    }
    for(i = 0; i != 18; ++i)
    {
        sortRet[sortMap[i]] = code[i].toAscii();
        buffer[i] = code[i].digitValue();
    }

    quint16 crcValue = ICUtility::CRC16(buffer,18);

    quint8 crc_Hi = crcValue >> 8 & 0xff;
    quint8 crc_Lo = crcValue & 0xff;
    if(crc_Hi > 9)
    {
        crc_Hi %= 9;
    }
    if(crc_Lo > 9)
    {
        crc_Lo %= 9;
    }
    if(crc_Hi != code[18].digitValue() || crc_Lo != code[19].digitValue())
    {
        return -1;
    }

    beg = 0;
    for(i = 0; i != 12; ++i)
    {
        beg += (machineCode[i].toAscii() - 48);
    }
    beg /= 10;
    beg %= 10;

    j = 0;
    for(i = beg; i < 10; ++i)
    {
        pMap[j++] = i;
    }
    for(i = 0; i < beg; ++i)
    {
        pMap[j++] = i;
    }

    for(i = 0; i != 18; ++i)
    {
        for(j = 0; j != 10; ++j)
        {
            if(j == (sortRet[i] - 48))
            {
                sortRet[i] = (pMap[j] + 48);
                break;
            }
        }
    }
    sortRet[18] = 0;
    for(i = 0; i != 12; ++i)
    {
        if(sortRet[i] != machineCode[i])
        {
            return -1;
        }
    }
    return atoi(sortRet + 12);
}

class CustomTabStyle : public QProxyStyle
{
public:
    //    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
    //                           const QSize &size, const QWidget *widget) const
    //    {
    //        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    //        if (type == QStyle::CT_TabBarTab)
    //            s.transpose();
    //        return s;
    //    }
    //    void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
    //    {
    //        if()
    //    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QStyleOptionTab opt(*tab);
                painter->save();
                //                painter->rotate(90);
                QProxyStyle::drawControl(element, &opt, painter, widget);
                painter->restore();
                return;
            }
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }
};

ICSettingsFrame::ICSettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICSettingsFrame)
{
    ui->setupUi(this);

    //    ui->tabWidget->addTab(new ICHCProductSettingFrame(), tr("Product Settings"));
    ui->tabWidget->insertTab(0, ICUpdateSystemPage::Instance(), tr("Maintain"));
    ui->tabWidget->insertTab(0, new ICMachineConfigPage(), tr("Axis Config"));
    ui->tabWidget->insertTab(0, new ICMachineStructPage(), tr("Machine Config"));
    ui->tabWidget->insertTab(0, new ICHCSystemSettingsFrame(), tr("Config"));
    ui->tabWidget->setCurrentIndex(0);
    //    ui->tabWidget->setStyle(new CustomTabStyle());

    //    ui->tabWidget->setTabShape(QTabWidget::Triangular);
    ui->fcode->setText(ICParametersSave::Instance()->FactoryCode());
}

ICSettingsFrame::~ICSettingsFrame()
{
    delete ui;
}

void ICSettingsFrame::showEvent(QShowEvent *e)
{
    ui->restTime->setText(QString::number(ICParametersSave::Instance()->RestTime(0) / 168.0) + tr("week"));
    QFrame::showEvent(e);
    if(ICParametersSave::Instance()->IsRoot())
    {
        ui->fcode->show();
        ui->fcCodeLabel->show();
    }
    else
    {
        ui->fcode->hide();
        ui->fcCodeLabel->hide();
    }
}

void ICSettingsFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICSettingsFrame::on_generateButton_clicked()
{
    QString ret;
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for(int i = 0; i != 6; ++i)
    {
        ret.append(QString::number(qrand() % 10));
    }
    ui->machineCode->setText(ret);
}

void ICSettingsFrame::on_registerButton_clicked()
{
    if(ui->machineCode->text().isNull())
    {
        ui->tipLabel->setText(tr("Wrong Register Code!"));
        return;
    }
    int hour = Register(ui->registerCode->text(), ui->fcode->text() + ui->machineCode->text());
    if(hour == -1)
    {
        ui->tipLabel->setText(tr("Wrong Register Code!"));
    }
    else
    {
        //        ICAppSettings().SetUsedTime(hour);
        ICParametersSave::Instance()->SetRestTime(hour * 168);
        ui->tipLabel->setText(tr("Register Success!"));
        if(hour == 0)
        {
            ui->restTime->setText(tr("No Limit"));
        }
        else
        {
            ui->restTime->setText(QString::number(hour) + tr("week"));
        }
        //        emit RegisterSucceed();
        ui->machineCode->clear();
        ui->registerCode->clear();

        //        ICDALHelper::UpdateConfigValue(ICAddr_System_OtherUsedTime, hour);
    }
    //    ICProgramHeadFrame::Instance()->ReashRestTime();
}

void ICSettingsFrame::on_fcode_textChanged(const QString &arg1)
{
    if(arg1.size() < 6)
    {
        ui->fcode->blockSignals(true);
        ui->fcode->setText(ICParametersSave::Instance()->FactoryCode());
        ui->fcode->blockSignals(false);
        return;
    }
    ICParametersSave::Instance()->SetFactoryCode(arg1);
}

void ICSettingsFrame::SetToShowAll(bool isshow)
{
    ui->tabWidget->setTabEnabled(0, isshow);
    ui->tabWidget->setTabEnabled(1, isshow);
    ui->tabWidget->setTabEnabled(2, isshow);
    ui->tabWidget->setTabEnabled(3, isshow);
}
