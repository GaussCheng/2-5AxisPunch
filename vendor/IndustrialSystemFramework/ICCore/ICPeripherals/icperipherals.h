///////////////////////////////////////////////////////////
//  icperipherals.h
//  Implementation of the Class ICPeripherals
//  Created on:      22-七月-2011 9:17:22
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#if !defined(EA_BC9B0E6F_B4F1_44be_8254_EECAB9C67C56__INCLUDED_)
#define EA_BC9B0E6F_B4F1_44be_8254_EECAB9C67C56__INCLUDED_

#include "ICCore_global.h"
#ifndef Q_WS_WIN
#include <sys/ioctl.h>
#endif
/**
 * @author GaussCheng
 * @version 1.0
 * @created 22-七月-2011 9:17:22
 */
class ICCORESHARED_EXPORT ICPeripherals
{

public:
    enum LedStatus{LedStatus_Off, LedStatus_On };
	ICPeripherals();
	virtual ~ICPeripherals();
	static void ICBacklightOff();
	static void ICBacklightOn();
	static void ICShowScreenSaver();
    static void SetLedStatus(int fd, LedStatus status, int whitch);
    static void SetLedStatus(int fd, int ledCode);

};

inline void ICPeripherals::SetLedStatus(int fd, LedStatus status, int whitch)
{
#ifndef Q_WS_WIN
    ioctl(fd, status, whitch);
#endif
}

inline void ICPeripherals::SetLedStatus(int fd, int ledCode)
{
#ifndef Q_WS_WIN
    ioctl(fd, 0, ledCode);
#endif
}

#endif // !defined(EA_BC9B0E6F_B4F1_44be_8254_EECAB9C67C56__INCLUDED_)
