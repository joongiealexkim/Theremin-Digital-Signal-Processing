/*
 * InitHardwareSubsystems.h
 *
 *  Created on: Sep 24, 2020
 *      Author: harry
 */

#ifndef INITHARDWARESUBSYSTEMS_H_
#define INITHARDWARESUBSYSTEMS_H_

#include "DAC.h"

#define TURN_OFF_ADC                    ADCCTL0 &= ~(ADCENC | ADCON | ADCSC)
#define DISABLE_MULTI_SAMP_CONVERSION   ADCCTL0 &= ~(ADCMSC)

#define SET_ADCSC_BIT_AS_SAMP_HOLD_SRC  ADCCTL1 &= ~(ADCSHS1 | ADCSHS0)
#define SAMP_SIGNAL_NON_INVERTED        ADCCTL1 &= ~(ADCISSH)
#define DIVIDE_ADC_CLK_BY_4             ADCCTL1 &= ~(ADCDIV2); ADCCTL1 |= (ADCDIV1 | ADCDIV0)
#define SET_CLK_SRC_AS_SMCLK            ADCCTL1 |= (ADCSSEL1 | ADCSSEL0)
#define SINGLE_CHAN_SING_CONV_MODE      ADCCTL1 &= ~(ADCCONSEQ1 | ADCCONSEQ0)
#define PULSE_SAMPLE_MODE               ADCCTL1 |= ADCSHP
#define PREDIVIDE_ADC_CLK_BY_4          ADCCTL2 |= ADCPDIV0; ADCCTL2 &= ~(ADCPDIV1)
#define SET_RESOLUTION_12_BIT           ADCCTL2 |= ADCRES1; ADCCTL2 &= ~ADCRES0
#define SET_DATA_FORMAT_UNSIGNED        ADCCTL2 &= ~(ADCDF)
#define SET_SAMPLING_RATE_200_KSPS      ADCCTL2 &= ~(ADCSR)
#define SET_REF_VOLTAGE_AVCC_AVSS       ADCMCTL0 &= ~(ADCSREF2 | ADCSREF1 | ADCSREF0)



#define SET_SWITCH_PIN_AS_INPUT         P1DIR &= ~(BIT3)
#define SET_SWITCH_PIN_AS_GPIO          P1SEL1 &= ~(BIT3);P1SEL0 &= ~(BIT3)
#define ENABLE_SWITCH_PULL_RESISTOR     P1REN |= BIT3
#define SELECT_PULL_DOWN_RESISTOR       P1OUT &= ~BIT3


//Initialize ADC and DAC
void init_HardwareSubsystems();

void init_ADC();

void init_DAC();

void init_Switch();


//DO NOT ENABLE Interrupts


#endif /* INITHARDWARESUBSYSTEMS_H_ */
