#ifndef ICUTILITY_GLOBAL_H
#define ICUTILITY_GLOBAL_H

#include <QtCore/qglobal.h>
#ifdef Q_WS_WIN32
#if defined(ICUTILITY_LIBRARY)
#  define ICUTILITYSHARED_EXPORT
#else
#  define ICUTILITYSHARED_EXPORT
#endif
#else
#if defined(ICUTILITY_LIBRARY)
#  define ICUTILITYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ICUTILITYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif

#endif // ICUTILITY_GLOBAL_H
