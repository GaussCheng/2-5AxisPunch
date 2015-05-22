#include "iccommonappsettings.h"
#include <QDir>

const char* ICCommonAppSettings::SystemConfigGroup = "CommonSystemConfig";
const char* ICCommonAppSettings::SessionGroup = "CommonSession";
const char* ICCommonAppSettings::LocaleGroup = "CommonLocale";
#ifdef Q_WS_X11
const char* ICCommonAppSettings::UsbPath = QDir::homePath().toUtf8();
#else
const char* ICCommonAppSettings::UsbPath = "/mnt/udisk";
#endif
QTime ICCommonAppSettings::startupTime_;
ICCommonAppSettings::ICCommonAppSettings()
{
}
