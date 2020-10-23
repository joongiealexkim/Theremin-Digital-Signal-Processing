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


#define ENABLE_SWITCH_PULL_RESISTOR     P1REN |= BIT3
#define SELECT_PULL_DOWN_RESISTOR       P1OUT &= ~BIT3


//Initialize ADC and DAC
void init_HardwareSubsystems();

void init_ADC();

void init_DAC();

void init_Switch();


//DO NOT ENABLE Interrupts


#endif /* INITHARDWARESUBSYSTEMS_H_ */
