#ifndef ICCONFIGFORMATCHECKER_H
#define ICCONFIGFORMATCHECKER_H

#include "icdataformatchecker.h"

class ICConfigFormatChecker : public ICDataFormatChecker
{
public:
    ICConfigFormatChecker();

protected:
    bool CheckImpl(const QString &dataStream);
};

#endif // ICCONFIGFORMATCHECKER_H
