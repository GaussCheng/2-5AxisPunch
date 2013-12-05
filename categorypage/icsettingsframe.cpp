#include "icsettingsframe.h"
#include "ui_icsettingsframe.h"
#include "icupdatesystem.h"
#include "icmachinestructpage.h"
#include "icmachineconfigpage.h"
#include "ichcproductsettingframe.h"
#include "ichcsystemsettingsframe.h"
#include <QProxyStyle>
#include <QPainter>

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
    ui->tabWidget->addTab(new ICHCSystemSettingsFrame(), tr("Config"));
    ui->tabWidget->addTab(new ICMachineStructPage(), tr("Machine Config"));
    ui->tabWidget->addTab(new ICMachineConfigPage(), tr("Axis Config"));
    ui->tabWidget->addTab(ICUpdateSystem::Instance(), tr("Maintain"));
//    ui->tabWidget->setStyle(new CustomTabStyle());

//    ui->tabWidget->setTabShape(QTabWidget::Triangular);
}

ICSettingsFrame::~ICSettingsFrame()
{
    delete ui;
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
