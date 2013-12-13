#include <QtGui/QApplication>
#include "mainframe.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"
#include "icsplashscreen.h"
#include "icparameterssave.h"
#include "icsystemconfig.h"

#ifdef Q_WS_WIN
#include <QFile>
#endif

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
#ifdef Q_WS_WIN
    QFile file("./stylesheet/global.qss");
    if(file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
#endif
//    qApp->setStyleSheet("./stylesheet/global.qss");
    ICParametersSave* paraSave = ICParametersSave::Instance();
    paraSave->SetCountry(paraSave->Country());
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
