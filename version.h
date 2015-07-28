#ifndef VERSION
#define VERSION
#include <QStringList>


enum MoldFunctionVersion    {
    VERSION_5_0_8,
    VERSION_5_0_9,
    VERSION_5_1_0,
    VERSION_5_1_1,
    VERSION_5_1_2,
    VERSION_lATEST
};

#define StackParamCount_ 10

#define VERSION_5_0_8_PT  3
#define VERSION_5_0_8_POINT 120
#define VERSION_5_0_8_FNC  (VERSION_5_0_8_POINT + 6 + StackParamCount_ * 4 + 1)

#define VERSION_5_0_9_PT  6
#define VERSION_5_0_9_POINT 240
#define VERSION_5_0_9_FNC  (VERSION_5_0_9_POINT + 6 + StackParamCount_ * 4 + 1)

#define VERSION_5_1_0_PT  6
#define VERSION_5_1_0_POINT 240
#define VERSION_5_1_0_FNC  (VERSION_5_1_0_POINT + 6 + StackParamCount_ * 4 + 1)

#define VERSION_5_1_1_PT  6
#define VERSION_5_1_1_POINT 240
#define VERSION_5_1_1_FNC  (VERSION_5_1_1_POINT + 6 + StackParamCount_ * 4 + 1)

#define VERSION_5_1_2_PT  6
#define VERSION_5_1_2_POINT 480
#define VERSION_5_1_2_FNC  (VERSION_5_1_2_POINT + 6 + StackParamCount_ * 4 + 1)


extern bool VersionCompatibleFNC(QStringList  &items,int &versoin);
extern bool VersionCompatiblePT(QStringList  &items);


#endif // VERSION

