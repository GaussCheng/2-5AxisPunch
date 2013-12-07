#include "icsettingsframe.h"
#include "ui_icsettingsframe.h"
#include "icupdatesystem.h"
#include "icmachinestructpage.h"
#include "icmachineconfigpage.h"
#include "ichcproductsettingframe.h"
#include "ichcsystemsettingsframe.h"
#include <QProxyStyle>
#include <QPainter>
#include <QDateTime>

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
    int sortMap[16];
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
    QString rC = code;
    QString sortRet(16, '0');
    int beg;
    if(rC.size() != 16)
    {
        return - 1;
    }
    for(int i = 0; i != sortRet.size(); ++i)
    {
        sortRet[sortMap[i]] = rC.at(i);
    }
    QString mCode = machineCode.simplified();
    mCode = mCode.remove(" ");
    beg = 0;
    for(int i = 0; i != mCode.size(); ++i)
    {
        if(!mCode.at(i).isDigit())
        {
            return -1;
        }
        beg += mCode.at(i).digitValue();
    }
    beg /= mCode.size();
    beg %= 10;
    int pMap[10];
    BuildShiftMap(beg, pMap);
    for(int i = 0; i != sortRet.size(); ++i)
    {
        for(int j = 0; j != 10; ++j)
        {
            if(j == sortRet.at(i).digitValue())
            {
                sortRet[i] = QString::number(pMap[j]).at(0);
                break;
            }
        }
    }
    if(sortRet.left(10) != mCode)
    {
        return -1;
    }
    //    return sortRet.right(6).toInt();
    return sortRet.right(6).toInt() * 24 * 7;
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
    ui->tabWidget->insertTab(0, ICUpdateSystem::Instance(), tr("Maintain"));
    ui->tabWidget->insertTab(0, new ICMachineConfigPage(), tr("Axis Config"));
    ui->tabWidget->insertTab(0, new ICMachineStructPage(), tr("Machine Config"));
    ui->tabWidget->insertTab(0, new ICHCSystemSettingsFrame(), tr("Config"));
    ui->tabWidget->setCurrentIndex(0);
    //    ui->tabWidget->setStyle(new CustomTabStyle());

    //    ui->tabWidget->setTabShape(QTabWidget::Triangular);
}

ICSettingsFrame::~ICSettingsFrame()
{
    delete ui;
}

void ICSettingsFrame::showEvent(QShowEvent *e)
{
    ui->restTime->setText(QString::number(ICParametersSave::Instance()->RestTime(0)) + tr("hour"));
    QFrame::showEvent(e);
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
    for(int i = 0; i != 10; ++i)
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
    int hour = Register(ui->registerCode->text(), ui->machineCode->text());
    if(hour == -1)
    {
        ui->tipLabel->setText(tr("Wrong Register Code!"));
    }
    else
    {
        //        ICAppSettings().SetUsedTime(hour);
        ICParametersSave::Instance()->SetRestTime(hour);
        ui->tipLabel->setText(tr("Register Success!"));
        if(hour == 0)
        {
            ui->restTime->setText(tr("No Limit"));
        }
        else
        {
            ui->restTime->setText(QString::number(hour) + tr("hour"));
        }
//        emit RegisterSucceed();
        ui->machineCode->clear();
        ui->registerCode->clear();

        //        ICDALHelper::UpdateConfigValue(ICAddr_System_OtherUsedTime, hour);
    }
//    ICProgramHeadFrame::Instance()->ReashRestTime();
}
