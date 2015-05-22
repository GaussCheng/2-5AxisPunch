#ifndef ICCONFIGPERMISSIONCENTER_H
#define ICCONFIGPERMISSIONCENTER_H

#include <QMap>
#include "icaddrwrapper.h"
#include "ICCore_global.h"

class ICCORESHARED_EXPORT ICConfigPermissionCenter
{
public:
    ICConfigPermissionCenter();

    static void FectchConfigPermissions();

    static int ConfigPermission(const ICAddrWrapper* config) { return configPermMap_.value(config, 0);}

private:
    static QMap<const ICAddrWrapper*, int> configPermMap_;
};

#endif // ICCONFIGPERMISSIONCENTER_H
