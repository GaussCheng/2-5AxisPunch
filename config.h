#ifndef CONFIG_H
#define CONFIG_H

#ifndef HC_8AXIS
#define HC_8AXIS
#endif

#ifdef HC_SK_8
#ifndef HC_ARMV6
#define HC_ARMV6
#endif
#endif

#define IFPOS_DECIMAL 0
#define POS_DECIMAL 1
#define SECTION_DECIMAL 1
#define STACK_DECIMAL 2

#define SW_VERSION_PREFIX "CYR"
#ifdef HC_SK_8
#define INCH "8"
#else
#define INCH "5"
#endif
#define HW_VERSION "1.3"

#define SW_VERSION "5.1.2"

static const char*  SHOW_VERSION =  SW_VERSION_PREFIX INCH"_"SW_VERSION;
#define UPDATE_PREFIX "HCPunch"

//#ifndef HC_SK_5
//#define HC_SK_5
//#endif

//#ifndef HC_SK_8
//#define HC_SK_8
//#endif

#endif // CONFIG_H
