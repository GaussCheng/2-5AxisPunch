#include <QtGui/QApplication>
#include "mainframe.h"
#include "icvirtualhost.h"
#include "ickeyboard.h"
#include "icsplashscreen.h"
#include "icparameterssave.h"

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
//    qApp->setStyleSheet("./stylesheet/global.qss");

    QPixmap splashPixmap("resource/startup_page.png");
    ICSplashScreen *splash= new ICSplashScreen(splashPixmap);
    splash->SetRange(0, 20);
    splash->show();

    a.setGlobalStrut(QSize(32, 32));
    ICParametersSave* paraSave = ICParametersSave::Instance();
    paraSave->LoadInitLocale();
    ICVirtualHost host;
    MainFrame w(splash);
    w.show();
    splash->finish(&w);
    delete splash;
    return a.exec();
}
