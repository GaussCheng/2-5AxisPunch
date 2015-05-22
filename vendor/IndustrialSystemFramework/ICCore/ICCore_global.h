#ifndef ICCORE_GLOBAL_H
#define ICCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ICCORE_LIBRARY)
#  define ICCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ICCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ICCORE_GLOBAL_H
