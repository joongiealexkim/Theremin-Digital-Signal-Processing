/*
 * ADC.h
 *
 *  Created on: Oct 23, 2020
 *      Author: user
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC_DATA                        ADCMEM0

#define TURN_OFF_ADC                    ADCCTL0 &= ~(ADCENC | ADCON | ADCSC)
#define TURN_ON_ADC                     ADCCTL0 |= (ADCON); ADCCTL0 |= (ADCSC | ADCENC)

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

// input A8
#define SET_ADC_INPUT_PITCH             ADCMCTL0 &= ~(ADCINCH2 | ADCINCH1 | ADCINCH0); ADCMCTL0 |= (ADCINCH3)
// input A9
#define SET_ADC_INPUT_VOLUME            ADCMCTL0 &= ~(ADCINCH2 | ADCINCH1); ADCMCTL0 |= (ADCINCH3 | ADCINCH0)
#define ADC_IS_BUSY                     ADCCTL1 & ADCBUSY

#define SENSOR_SAMPLE_PERIOD            40000 // (16 MHz/8) / 40000 cycles = 50 Hz, or 50 samples per second

void ConfigureTimerB1();

#endif /* ADC_H_ */
