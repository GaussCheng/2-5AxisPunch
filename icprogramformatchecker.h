#ifndef ICPROGRAMFORMATCHECKER_H
#define ICPROGRAMFORMATCHECKER_H

#include "icdataformatchecker.h"

class ICProgramFormatChecker : public ICDataFormatChecker
{
public:
    ICProgramFormatChecker();

protected:
    virtual bool CheckImpl(const QString &dataStream);
};

#endif // ICPROGRAMFORMATCHECKER_H
