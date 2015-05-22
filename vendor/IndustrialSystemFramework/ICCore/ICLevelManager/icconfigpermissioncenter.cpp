#include "icconfigpermissioncenter.h"
#include <QSqlQuery>
#include <QVariant>

QMap<const ICAddrWrapper*, int> ICConfigPermissionCenter::configPermMap_;

ICConfigPermissionCenter::ICConfigPermissionCenter()
{
}

void ICConfigPermissionCenter::FectchConfigPermissions()
{
    QSqlQuery query;
    query.exec("SELECT * FROM tb_config_permission");
    while(query.next())
    {
        configPermMap_.insert(ICAddrWrapper::AddrStringToAddr(query.value(0).toString()), query.value(1).toInt());
    }
}
