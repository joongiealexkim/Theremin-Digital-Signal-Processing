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

#define DISABLE_OPAMP                   SAC_OPAMP_RGSTR &= ~(SACEN)
#define OPAMP_NON_INVERTED_BUFFER_MODE  SAC_OPAMP_RGSTR &= ~(NSEL1); SAC_OPAMP_RGSTR |= (NSEL0); SAC_PGA_RGSTR &= ~(MSEL1); SAC_PGA_RGSTR |= (MSEL0); SAC_OPAMP_RGSTR &= ~(PSEL1); SAC_OPAMP_RGSTR |= (PSEL0)
#define ENABLE_OPAMP_INPUTS             SAC_OPAMP_RGSTR |= PMUXEN | NMUXEN
#define DISABLE_DAC                     SAC_DAC_RGSTR &= ~(DACEN)
#define DAC_SET_PRIMARY_REF_VOLTAGE     SAC_DAC_RGSTR &= ~(DACSREF)
#define DAC_LATCH_LOAD_ON_WRITE         SAC_DAC_RGSTR &= ~(DACLSEL1 | DACLSEL0)


/*
 * Given an input distance measurement, returns the frequency of the note that corresponds to that distance.
 * The frequency is calculated with the formula:
 * f_n = f_0 * (a)^n
 * Where
 * f_n is the output frequency
 * f_0 is the frequency of our fixed note
 * a is what I'll call the frequency base - it is equal to the twelfth root of two
 * n is the number of half steps away from our fixed note our note is.
 *
 * We calculate n by doing the following:
 * We define the minimum and maximum frequency that our system will output.
 * We can solve the above formula for n, giving us the number of steps that correspond to the minimum and maximum frequency.
 * That calculation is done in the initialization function, init_sine_gen().
 *
 * Since our distance data must be between 0 and 4095 (inclusive),
 * we say that a value of 0 will correspond to the minimum frequency,
 * while a value of 4095 will correspond to the maximum frequency.
 * We thus use n to linearly interpolate across the otherwise exponential function.
 *
 * Arguments:
 * unsigned int distance_data - the 12-bit number corresponding to the reading from the optical sensor assigned to pitch.
 *
 * Returns:
 * The frequency that corresponds to the value of range_data, interpolated within the frequency domain.
 */
unsigned int calc_freq(unsigned int distance_data);

/*
 * Calculates the period for which DAC will update values.
 * Since the ultimate goal is a sine wave, we need to multiply the frequency by the number of entries
 * in our sine lookup table.
 * However, for higher frequencies, we also want to skip over entries in the sine lookup table to ensure that
 * we don't struggle to keep up with updating the DAC. On the other hand, we want to ensure that there's a minimum frequency
 * so that our reconstruction filter works properly.
 *
 */
unsigned int calc_period(unsigned int frequency);

void init_sine_gen();





#endif /* DAC_H_ */
