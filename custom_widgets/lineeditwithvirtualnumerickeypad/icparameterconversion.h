#ifndef ICPARAMETERCONVERSION_H
#define ICPARAMETERCONVERSION_H

#include <QString>

class ICParameterConversion
{
public:
    ICParameterConversion();

    static QString TransThisIntToThisText(int inputNum, int decimals = 0);
    static int TransTextToThisInt(const QString &numString, int decimals = 0);

private:
    static int Pow(int variable, int power);
};

#endif // ICPARAMETERCONVERSION_H
