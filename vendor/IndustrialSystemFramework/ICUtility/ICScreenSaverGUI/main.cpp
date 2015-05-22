#include <QtGui/QApplication>
#include "icscreensavergui.h"
#include "icutility.h"


int main(int argc, char *argv[])
{
    if(!ICUtility::LockFile(ICUtility::ScreenSaverInstanceLockFile))
    {
        exit(1);
    }
    QApplication a(argc, argv);
    ICScreenSaverGUI w;
    w.show();

    return a.exec();
}
