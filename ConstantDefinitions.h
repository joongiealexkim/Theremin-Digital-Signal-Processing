/*
 * ConstantDefinitions.h
 *
 *  Created on: Sep 24, 2020
 *      Author: harry
 */

#ifndef CONSTANTDEFINITIONS_H_
#define CONSTANTDEFINITIONS_H_

#define FIXED_NOTE_FREQ     440 // A4 = 440 Hz
#define MIN_FREQ            220 // A3 = 220 Hz
#define MAX_FREQ            1760 // A6 = 1760 Hz
#define FREQ_BASE           1.05946309 // a = 2^(1/12), the twelfth root of two, to 8 decimal places.
#define TARGET_MIN_FREQ     14080

int freq_step_range;
#define SINE_ARRAY_LENGTH   64


#endif /* CONSTANTDEFINITIONS_H_ */
