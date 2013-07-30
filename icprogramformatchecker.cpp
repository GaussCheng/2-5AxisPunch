#include "icprogramformatchecker.h"
#include <QStringList>

ICProgramFormatChecker::ICProgramFormatChecker()
{
}

bool ICProgramFormatChecker::CheckImpl(const QString &dataStream)
{
    if(!CheckRowCount(dataStream, 1, ICDataFormatChecker::kCompareLarggerOrEqual))
    {
        return false;
    }
    if(!CheckColumCount(dataStream, -1, " ", 10, ICDataFormatChecker::kCompareLarggerOrEqual))
    {
        return false;
    }
    if(!CheckDataVal(dataStream, -1, 3, " ", "32"))
    {
        return false;
    }

    QString temp = dataStream;
    temp.remove(" ");
    temp = temp.remove("\n");
    for(int i = 0; i != temp.length(); ++i)
    {
        if(!temp.at(i).isDigit())
        {
            return false;
        }
    }
    return true;
}
