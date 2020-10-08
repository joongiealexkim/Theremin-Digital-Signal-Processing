/*
 * sine_gen.c
 *
 *  Created on: Sep 25, 2020
 *      Author: user
 */

#include <msp430.h>
#include "sine_gen.h"
#include <math.h>

/*
 * Initialize pins and variables.
 */
void init_sine_gen() {
    min_freq_steps = log(MIN_FREQ/FIXED_NOTE_FREQ)/log(FREQ_BASE);
    max_freq_steps = log(MAX_FREQ/FIXED_NOTE_FREQ)/log(FREQ_BASE);
}

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
double calc_freq(unsigned int distance_data) {
    double n;

    // We develop our linear interpolation within our frequency domain and multiply it by our range_data
    n = (double) distance_data * (max_freq_steps - min_freq_steps)/(4096);

    // now we can go back and calculate the corresponding frequency
    return FIXED_NOTE_FREQ * pow(FREQ_BASE, n);
}

/**
 * This here is modified code from ECE 3430, Intro to Embedded Design
 * The current plan is to modify the frequency on-the-fly
 */
/*
void ConfigureTimerA0(void)
{
    // Stop the Timer A0 and clear it.
    TA0CTL = (MC_0 | TACLR);
    // Configure the timer, including input clock select, input clock divide,
    // and starting the timer in up mode.
    TA0CTL |= (TASSEL_2 | ID_0 | MC_1);
    // Initialize TA0CCR0.
    TA0CCR0 = 1136; // For a 16 MHz clock, this corresponds to a frequency of 440*32 Hz.
    // Enable TA0CCR0 Compare/Capture Interrupt Flag (CCIFG0)
    TA0CCTL0 |= CCIE;
}
*/

/*
#pragma vector = TIMER0_A0_VECTOR
// Interrupt service routine for CCIFG0
__interrupt void TimerA0ISR(void)
{
    // somehow get the current volume range data from the ADC
    // it will be a 12-bit number, so we can cast it to double and multiply it by the sine_function
    // BUT that number will then vary from 0-4096, and that's too high because we're going to be adding the DC offset    
    // SO we divide by 2 before casting it back to an int    
    // the result will be a 12-bit number.
    double scaled_result;
    scaled_result = (double) volume_range_data * SinusoidArray[CurrentArrayIndex] / 2;
    double shifted_result;
    shifted_result = scaled_result + 2047.5; //Our output needs to be between 0 and 4095, inclusive

    DACDAT = (int) shifted_result; //truncate and throw into the DAC
}
*/

