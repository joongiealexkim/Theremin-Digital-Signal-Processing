/*
 * sine_gen.h
 *
 *  Created on: Sep 24, 2020
 *      Author: Alex Kim
 */

#ifndef DAC_H_
#define DAC_H_

// Since we are using p3.1 as the DAC output, we need to configure SAC2.
#define SAC_OPAMP_RGSTR                 SAC2OA
#define SAC_PGA_RGSTR                   SAC2PGA
#define SAC_DAC_RGSTR                   SAC2DAC
#define SAC_DATA_RGSTR                  SAC2DAT

#define DISABLE_OPAMP                   SAC_OPAMP_RGSTR &= ~(SACEN); SAC_OPAMP_RGSTR &= ~(OAEN)
#define ENABLE_OPAMP                    SAC_OPAMP_RGSTR |=  (SACEN); SAC_OPAMP_RGSTR |=  (OAEN)
#define OPAMP_NON_INVERTED_BUFFER_MODE  SAC_OPAMP_RGSTR &= ~(NSEL1); SAC_OPAMP_RGSTR |= (NSEL0); SAC_PGA_RGSTR &= ~(MSEL1); SAC_PGA_RGSTR |= (MSEL0); SAC_OPAMP_RGSTR &= ~(PSEL1); SAC_OPAMP_RGSTR |= (PSEL0)
#define ENABLE_OPAMP_INPUTS             SAC_OPAMP_RGSTR |= PMUXEN | NMUXEN
#define DISABLE_DAC                     SAC_DAC_RGSTR &= ~(DACEN)
#define ENABLE_DAC                      SAC_DAC_RGSTR |= (DACEN)
#define DAC_SET_PRIMARY_REF_VOLTAGE     SAC_DAC_RGSTR &= ~(DACSREF)
#define DAC_LATCH_LOAD_ON_WRITE         SAC_DAC_RGSTR &= ~(DACLSEL1 | DACLSEL0)

/**
 * Given an input 12-bit unsigned integer, returns the corresponding Timer Capture Register value in order to produce the desired pitch for a 16 MHz clock.
 * The results of this function are based upon the equal tempered scale function; however, to increase speed, all possible values are pre-calculated and stored in lookup tables.
 * The output of the equal tempered scale function is linearly mapped across three octaves (220 Hz to 1760 Hz).
 * Values in each octave are then additionally multiplied by a different number in order to ensure that the DAC interrupt does not occur too often;
 * to compensate, the number of samps_per_cycle is divided by the same factor.
 *
 * If discrete is equal to 0, this function is a 1-1 mapping of all 4095 possible distance_data values to 4095 corresponding Timer Capture Register values,
 *
 * If discrete is nonzero, this function rounds values down to the nearest note on the 12-note scale.
 */
unsigned int calc_period_full(unsigned int distance_data, char discrete);

void ConfigureTimerB0();

extern const int discretePeriod[];

extern const int continuousPeriod[];

extern const int powTable[];

extern const int SinusoidArrayInt[];

#endif /* DAC_H_ */
