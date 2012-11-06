#include "icconfigformatchecker.h"

ICConfigFormatChecker::ICConfigFormatChecker()
{
}

bool ICConfigFormatChecker::CheckImpl(const QString &dataStream)
{
    if(!CheckColumCount(dataStream, -1, " ", 1, kCompareEqual))
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
