#include "icaddrwrapper.h"

QStringList ICAddrWrapper::typeStringList_ = QStringList()<<"n"<<"c"<<"m"<<"s"<<"b";
QStringList ICAddrWrapper::permissionStringList_ = QStringList()<<"n"<<"ro"<<"wo"<<"rw";
QMap<QString, const ICAddrWrapper*> ICAddrWrapper::addrStringToAddrMap_;

ICAddrWrapper::ICAddrWrapper()
{
}

ICAddrWrapper::ICAddrWrapper(int type, int perm, int startPos, int size, int baseAddr, int decimal, const QString &unit)
{
    if(type < kICAddrTypeNoUse ||
            type > kICAddrTypeBadAddr ||
            perm < kICAddrPermissionNone ||
            type > kICAddrPermissionRW ||
            startPos < 0 ||
            startPos >= ICADDR_BIT_WIDTH ||
            size < 0 ||
            size > ICADDR_BIT_WIDTH) type = kICAddrTypeBadAddr;
//    addr_.orginAddr = 0;
    addr_.addrType = type;
    addr_.perm = perm;
    addr_.startPos = startPos;
    addr_.size = size;
    addr_.baseAddr = baseAddr;
    addr_.decimal = decimal;
    unit_ = unit;
    addrStringToAddrMap_.insert(this->ToString(), this);
}
