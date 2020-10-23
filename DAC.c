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

/*
 * Initialize pins and variables.
 */
void init_sine_gen() {

}

unsigned int calc_freq(unsigned int distance_data) {
    double n;

    // We develop our linear interpolation within our frequency domain and multiply it by our distance_data
    n = (double) ((distance_data * freq_step_range) >> 12);
    // note: bit shifting right by 12 places is equivalent to dividing by 4096

    // now we can go back and calculate the corresponding frequency
    return (unsigned int) FIXED_NOTE_FREQ * pow(FREQ_BASE, n);
}



unsigned int calc_period(unsigned int frequency) {
    if (frequency < MIN_FREQ || frequency > MAX_FREQ) {
        return 0;
    }

    // Hard coding these ranges for speed, as calculating what I want it to could take some time.
    // Definitely can be improved.
    if (frequency < 440) samps_per_cycle = 64;
    else if (frequency < 880) samps_per_cycle = 32;
    else if (frequency < 1760) samps_per_cycle = 16;
    else samps_per_cycle = 8;

    return (unsigned int) (16000000 / (frequency * samps_per_cycle));

}


/**
 * This here is modified code from ECE 3430, Intro to Embedded Design
 * The current plan is to modify the frequency on-the-fly
 */

void ConfigureTimerB0(void)
{
    // Stop the Timer B0 and clear it.
    TB0CTL = (MC_0 | TBCLR);

    // Configure the timer, including input clock select, input clock divide,
    // and starting the timer in up mode.
    TB0CTL |= (TBSSEL_2 | ID_0 | MC_1);
    // Initialize TB0CCR0.
    TB0CCR0 = DAC_period; // arbitrary value

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


