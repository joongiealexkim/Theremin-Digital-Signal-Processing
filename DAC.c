/*
 * sine_gen.c
 *
 *  Created on: Sep 25, 2020
 *      Author: user
 */

#include <DAC.h>
#include <msp430.h>
#include <math.h>
#include "ConstantDefinitions.h"
#include "GlobalVariables.h"

unsigned int calc_freq(unsigned int distance_data) {
    double n;
    double freq;

    // We develop our linear interpolation within our frequency domain and multiply it by our distance_data
    n = (double) ((distance_data * freq_step_range) >> 12)+(min_freq_steps);
        // note: bit shifting right by 12 places is equivalent to dividing by 4096,

    // now we can go back and calculate the corresponding frequency
    freq = FIXED_NOTE_FREQ * pow((double) FREQ_BASE, n);
    return (unsigned int) freq;
}

unsigned int calc_period(unsigned int frequency) {
    double period;

    if (frequency < MIN_FREQ || frequency > MAX_FREQ) {
        return 0;
    }

    // Hard coding these ranges for speed, as calculating what I want it to could take some time.
    // Definitely can be improved.
    if (frequency < 440) samps_per_cycle = 64;
    else if (frequency < 880) samps_per_cycle = 32;
    else if (frequency < 1760) samps_per_cycle = 16;
    else samps_per_cycle = 8;

    // The period is (clock speed) / (frequency * samples per cycle)
    period =  (16000000 / (frequency * samps_per_cycle));
    return (unsigned int) period;
}

void ConfigureTimerB0(void)
{
    // Stop the Timer B0 and clear it.
    TB0CTL = (MC_0 | TBCLR);

    // Configure the timer, including input clock select, input clock divide,
    // and starting the timer in up mode.
    TB0CTL |= (TBSSEL_2 | ID_0 | MC_1);
    // Initialize TB0CCR0.
    TB0CCR0 = DAC_period;

    // set Timer B0 to Compare mode
    TB0CCTL0 &= ~(CAP);
    // Enable TB0CCR0 Compare/Capture Interrupt Flag (CCIFG0)
    TB0CCTL0 |= CCIE;
}


#pragma vector = TIMER0_B0_VECTOR
// Interrupt service routine for CCIFG0
__interrupt void TimerB0ISR(void)
{
    TB0CCR0 = DAC_period;
    SAC_DATA_RGSTR = next_sine_value;

    // step forward in the array index
    current_array_index += SINE_ARRAY_LENGTH/samps_per_cycle;
    current_array_index = current_array_index % SINE_ARRAY_LENGTH;

}

// The sine lookup table.
// I wanted to put this in constant definitions, but this caused a linking error because it would be defined multiple times.
// For lack a better way, I am putting it in here instead.
const double SinusoidArray[64] = {
1,
0.995184727,
0.98078528,
0.956940336,
0.923879533,
0.881921264,
0.831469612,
0.773010453,
0.707106781,
0.634393284,
0.555570233,
0.471396737,
0.382683432,
0.290284677,
0.195090322,
0.09801714,
6.12574E-17,
-0.09801714,
-0.195090322,
-0.290284677,
-0.382683432,
-0.471396737,
-0.555570233,
-0.634393284,
-0.707106781,
-0.773010453,
-0.831469612,
-0.881921264,
-0.923879533,
-0.956940336,
-0.98078528,
-0.995184727,
-1,
-0.995184727,
-0.98078528,
-0.956940336,
-0.923879533,
-0.881921264,
-0.831469612,
-0.773010453,
-0.707106781,
-0.634393284,
-0.555570233,
-0.471396737,
-0.382683432,
-0.290284677,
-0.195090322,
-0.09801714,
-1.83772E-16,
0.09801714,
0.195090322,
0.290284677,
0.382683432,
0.471396737,
0.555570233,
0.634393284,
0.707106781,
0.773010453,
0.831469612,
0.881921264,
0.923879533,
0.956940336,
0.98078528,
0.995184727,
};
