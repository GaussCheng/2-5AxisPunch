#include "icsplashscreen.h"
#include "config.h"
#include "icparameterssave.h"

#include <QFormLayout>


ICSplashScreen::ICSplashScreen(const QPixmap &pixmap, Qt::WindowFlags f) :
    QSplashScreen(pixmap, f)
{
    progressBar_ = new QProgressBar(this);
    progressBar_->resize(this->width() - 200, 20);
    progressBar_->move(100, height() * 7 / 8 );
    versionContainer_ = new QWidget(this);
    QFormLayout *flayout = new QFormLayout();
    swVersionLabel_ = new QLabel(tr("SW Version:"), this);
    swVersion_ = new QLabel(SHOW_VERSION, this);
    hwVersionLabel_ = new QLabel(tr("HW Version:"), this);
    hwVersion_ = new QLabel(HW_VERSION, this);
    clientLabel_ = new QLabel(tr("Client:"), this);
    client_ = new QLabel(QString::fromUtf8(ICParametersSave::Instance()->Client().toLatin1()), this);
//    client_ = new QLabel(ICParametersSave::Instance()->Client(), this);
    if(client_->text().isEmpty())
    {
        clientLabel_->hide();
        client_->hide();
    }
    dateLabel_ = new QLabel(tr("Shiment Date:"), this);
    date_ = new QLabel(ICParametersSave::Instance()->ShipmentDate(), this);
    if(date_->text().isEmpty())
    {
        dateLabel_->hide();
        date_->hide();
    }
    flayout->addRow(swVersionLabel_, swVersion_);
    flayout->addRow(hwVersionLabel_, hwVersion_);
    flayout->addRow(clientLabel_, client_);
    flayout->addRow(dateLabel_, date_);

    versionContainer_->setLayout(flayout);

    int h = height() * 0.65;
    versionContainer_->move(20, h);
    connect(this,
            SIGNAL(messageChanged(QString)),
            this,
            SLOT(OnMessageChanged(QString)));
}

void ICSplashScreen::OnMessageChanged(const QString &message)
{
    Q_UNUSED(message)
    int v = progressBar_->value() + 1;
    progressBar_->setValue(v);
}
