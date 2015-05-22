///////////////////////////////////////////////////////////
//  icperipherals.cpp
//  Implementation of the Class ICPeripherals
//  Created on:      22-七月-2011 9:17:23
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#include <cstdlib>
#include "icperipherals.h"


ICPeripherals::ICPeripherals(){

}



ICPeripherals::~ICPeripherals(){

}


/**
 * 灭背光
 */
void ICPeripherals::ICBacklightOff()
{
    system("Backlight off");
}


/**
 * 亮背光
 */
void ICPeripherals::ICBacklightOn()
{
    system("Backlight on");
}


/**
 * 显示屏保
 */
void ICPeripherals::ICShowScreenSaver()
{
    system("ICScreenSaverGUI -qws");
}
