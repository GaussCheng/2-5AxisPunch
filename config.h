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

#ifdef HC_SK_8
#define SW_VERSION "CYR8_5.0.3"
#else
#define SW_VERSION "CYR5_5.0.3"
#endif
#define HW_VERSION "1.3"

//#ifndef HC_SK_5
//#define HC_SK_5
//#endif

//#ifndef HC_SK_8
//#define HC_SK_8
//#endif

#endif // CONFIG_H
