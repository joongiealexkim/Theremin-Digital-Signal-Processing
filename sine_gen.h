/*
 * sine_gen.h
 *
 *  Created on: Sep 24, 2020
 *      Author: Alex Kim
 */

#ifndef SINE_GEN_H_
#define SINE_GEN_H_

#define FIXED_NOTE_FREQ     440 // A4 = 440 Hz
#define MIN_FREQ            220 // A3 = 220 Hz
#define MAX_FREQ            3520 // A7 = 3520 Hz
#define FREQ_BASE           1.05946309 // a = 2^(1/12), the twelfth root of two, to 8 decimal places.

// The sine lookup table.
const double SinusoidArray[32] = {
1,
0.98078528,
0.923879533,
0.831469612,
0.707106781,
0.555570233,
0.382683432,
0.195090322,
0,
-0.195090322,
-0.382683432,
-0.555570233,
-0.707106781,
-0.831469612,
-0.923879533,
-0.98078528,
-1,
-0.98078528,
-0.923879533,
-0.831469612,
-0.707106781,
-0.555570233,
-0.382683432,
-0.195090322,
0,
0.195090322,
0.382683432,
0.555570233,
0.707106781,
0.831469612,
0.923879533,
0.98078528,
};
unsigned int SinusoidArrayLength = 32;
unsigned int CurrentArrayIndex = 0;

double min_freq_steps; //the number of half-steps away the minimum frequency is from the fixed note frequency
double max_freq_steps; //the number of half-steps away the maximum frequency is from the fixed note frequency

void init_sine_gen();

double calc_freq(unsigned int range_data);


#endif /* SINE_GEN_H_ */
