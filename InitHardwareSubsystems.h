/*
 * InitHardwareSubsystems.h
 *
 *  Created on: Sep 24, 2020
 *      Author: harry
 */

#ifndef INITHARDWARESUBSYSTEMS_H_
#define INITHARDWARESUBSYSTEMS_H_

#include "DAC.h"
#include "ADC.h"
#include "PortIODefs.h"




//Initialize ADC and DAC
void init_HardwareSubsystems();

void init_Sensor_GPIO();

void init_ADC();

void init_DAC();

void init_Switch();


//DO NOT ENABLE Interrupts


#endif /* INITHARDWARESUBSYSTEMS_H_ */
