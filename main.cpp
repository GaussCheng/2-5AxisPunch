#include <QtGui/QApplication>
#include "mainframe.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"
#include "icsplashscreen.h"
#include "icparameterssave.h"
#include "icsystemconfig.h"
#include <QSqlDatabase>
#include "icmessagebox.h"
#include <QTextCodec>

//#ifdef Q_WS_WIN
#include <QFile>
//#endif

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QTextCodec *textc = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(textc);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("3-5AxisRobotDatabase");
    if(!db.isValid())
    {
        qCritical("Open Database fail!!");
        ICMessageBox::ICWarning(NULL, QT_TR_NOOP("Error"), QT_TR_NOOP("Database is error!!"));
    }
    if(!db.open())
    {
        qCritical("Open Database fail!!");
        ICMessageBox::ICWarning(NULL, QT_TR_NOOP("Error"), QT_TR_NOOP("Open Database fail!!"));
    }
#ifndef Q_WS_QWS
    QFile file("./stylesheet/global.qss");
    if(file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
#endif
//    qApp->setStyleSheet("./stylesheet/global.qss");
    ICParametersSave* paraSave = ICParametersSave::Instance();
    paraSave->SetCountry(paraSave->Country(), false);
    paraSave->SetBrightness(paraSave->Brightness(), false);
    ICUserDefineConfig::Instance()->Init();
    QPixmap splashPixmap("resource/startup_page.png");
#ifdef HC_SK_5
    splashPixmap = splashPixmap.scaled(640, 480);
#endif
    ICSplashScreen *splash= new ICSplashScreen(splashPixmap);
    splash->SetRange(0, 20);
    splash->show();

    a.setGlobalStrut(QSize(32, 32));
    paraSave->LoadInitLocale();
    ICVirtualHost host;
    MainFrame w(splash);
    w.show();
//    splash->finish(&w);
    delete splash;
    return a.exec();
}
