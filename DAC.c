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
#include "PortIODefs.h"
#include "test_functions.h"
#include "IQmathLib.h"


unsigned int calc_freq(unsigned int distance_data, char discrete) {
    double n;

    unsigned long volatile l;
    unsigned long volatile l2;

    //hardware multiplication
    //MPY32CTL0 |= MPYM_0;

    discrete=0;
    // We develop our linear interpolation within our frequency domain and multiply it by our distance_data
    if (discrete) {
        unsigned int n_discrete;
        MPY = distance_data + 1;
        OP2 = freq_step_range;

        //The hardware multiplier requires 3 cycles after writing OP2 to complete the multiplication
        __delay_cycles(3);

        //read the result of the hardware multiplication
        l = ((unsigned long) RES1 << 16) | (RES0);

//        n = (double) (l >> 12) + (min_freq_steps);
        n_discrete = (l >> 12);
        // note: bit shifting right by 12 places is equivalent to dividing by 4096,
        // but it removes all decimals.

        unsigned int pow_output;
        pow_output = powTable[n_discrete];
        MPY = pow_output;
        OP2 = FIXED_NOTE_FREQ;

        unsigned int volatile l3;

        l3 = (unsigned int) ((((unsigned long) RES1 << 16) | (RES0)) >> 12);

        return l3;

    }
    else {
        MPY = (distance_data + 1);
        OP2 = freq_step_range;

        //The hardware multiplier requires 3 cycles after writing OP2 to complete the multiplication
        __delay_cycles(3);

        //read the result of the hardware multiplication
        l2 = ((unsigned long) RES1 << 16) | (RES0) ;

        _iq12 volatile long2 = l2 << 12;
        MPY32L = (int) (long2 & 0xFFFF);
        MPY32H = (int) (long2 >> 16);
        OP2L = (int) (hardware_range & 0xFFFF);
        OP2H = (int) (hardware_range >> 16);
        __delay_cycles(11);
        //32bit iq12 * 32bit iq12 = 64bit iq24
        //we know for a fact that the integer portion is going to be in the range of 0-36, so we only need to read the lower 32 bits
        _iq24 volatile output = ((unsigned long) RES1 << 16) | RES0;
        _iq24 volatile minfreqsteps = (long) min_freq_steps << 24;
        _iq24 volatile n24 = output + minfreqsteps;
        _iq24 volatile result;

        _iq24 volatile freq_base_log = _IQ24log(freq_base_fixed); // i imagine there's some way to speed this up
        //the result of this operation is SIGNED, by the way.
        // so when we multiply, we need to get a signed value out of it.

        MPYS32L = (int) (freq_base_log & 0xFFFF);
        MPYS32H = (int) (freq_base_log >> 16);
        OP2L = (int) (n24 & 0xFFFF);
        OP2H = (int) (n24 >> 16);
        __delay_cycles(11);
        //32 iq24 * 32bit iq24 = 64bit iq48
        //we know for a fact that the integer portion is going to be far less than 16 bits, so
        //we read the 16 integer bits and the top 16 fractional bits
        _iq16 volatile log_result = (unsigned long) RES3 << 16) | RES2;

        result = _IQ16exp(log_result); //i imagine there's some way to speed this up

        MPY32L = (int) (fixed_note_freq_fixed & 0xFFFF);
        MPY32H = (int) (fixed_note_freq_fixed >> 16);
        OP2L = (int) (result) & 0xFFFF;
        OP2H = (int) (result >> 16);
        __delay_cycles(11);
        //32bit iq16 * 32bit iq16 = 64bit iq32
        //we know for a fact that the integer portion is going to be in the range of 220-1760
        //so we only need to read the least significant integer bits!
        unsigned int volatile final = RES2;
        return final;

        // exp(log(x) * n) == pow(x,n)
//        n = (double) (l / 4096.0) + (min_freq_steps);
        // floating point division is awfully slow. I really would like to improve this somehow.
    }

    // now we can go back and calculate the corresponding frequency


    return (unsigned int) FIXED_NOTE_FREQ * pow((double) FREQ_BASE, n);
}

unsigned int calc_hack(unsigned int distance_data, char discrete) {
    int n;

    unsigned long volatile l;
    unsigned long volatile l2;

    //hardware multiplication
    MPY32CTL0 |= MPYM_0;



    // We develop our linear interpolation within our frequency domain and multiply it by our distance_data
    if (discrete) {
        unsigned int n_discrete;
        MPY = distance_data + 1;
        OP2 = freq_step_range;

        //The hardware multiplier requires 3 cycles after writing OP2 to complete the multiplication
        __delay_cycles(3);

        //read the result of the hardware multiplication
        l = ((unsigned long) RES1 << 16) | (RES0);

//        n = (double) (l >> 12) + (min_freq_steps);
        n_discrete = (l >> 12);
        // note: bit shifting right by 12 places is equivalent to dividing by 4096,
        // but it removes all decimals.

        unsigned int pow_output;
        pow_output = powTable[n_discrete];
        MPY32CTL0 |= MPYM_0;
        MPY = pow_output;
        OP2 = FIXED_NOTE_FREQ;

        unsigned int volatile l3;

        l3 = (unsigned int) ((((unsigned long) RES1 << 16) | (RES0)) >> 12);

        if (l3 < MIN_FREQ || l3 > MAX_FREQ) {
            return 0;
        }

        // Hard coding these ranges for speed, as calculating what I want it to could take some time.
        // Definitely can be improved.
        // starting spc - 64
        if (l3 < 440) samps_per_cycle = 64;
        else if (l3 < 880) samps_per_cycle = 32;
        else if (l3 < 1760) samps_per_cycle = 16;
        else samps_per_cycle = 8;

        // The period is (clock speed) / (frequency * samples per cycle)
        return discretePeriod[n_discrete];

    }


    return (unsigned int) FIXED_NOTE_FREQ * pow((double) FREQ_BASE, n);
}

unsigned int calc_period(unsigned int frequency) {

    if (frequency < MIN_FREQ || frequency > MAX_FREQ) {
        return 0;
    }

    // Hard coding these ranges for speed, as calculating what I want it to could take some time.
    // Definitely can be improved.
    // starting spc - 64
    if (frequency < 440) samps_per_cycle = 64;
    else if (frequency < 880) samps_per_cycle = 32;
    else if (frequency < 1760) samps_per_cycle = 16;
    else samps_per_cycle = 8;

    // The period is (clock speed) / (frequency * samples per cycle)
    return (unsigned int) (16000000 / (frequency * samps_per_cycle));
}

void ConfigureTimerB0(void)
{
    // Stop the Timer B0 and clear it.
    TB0CTL = (MC_0 | TBCLR);

    // set Capture/Compare Register 0 to Compare mode
    TB0CCTL0 &= ~(CAP);

    // Initialize TB0CCR0.
    TB0CCR0 = DAC_period;

    // Configure the timer, including input clock select, input clock divide,
    // and starting the timer in up mode.
    TB0CTL |= (TBSSEL_2 | ID_0 | MC_1);

    // Enable TB0CCR0 Compare/Capture Interrupt Flag (CCIFG0)
    TB0CCTL0 |= CCIE;
}


#pragma vector = TIMER0_B0_VECTOR
// Interrupt service routine for CCIFG0
__interrupt void TimerB0ISR(void)
{
    signed long volatile l_amplitude;

//    if (SWITCH_IS_ON) DAC_period = calc_hack(pitch_range_data, DISCRETE);
//    else DAC_period = calc_period(calc_freq(pitch_range_data, CONTINUOUS));

    DAC_period = calc_period(calc_freq(pitch_range_data, CONTINUOUS));

    TB0CCR0 = DAC_period;
//    scaled_result = (double) volume_range_data * SinusoidArray[current_array_index] / 2;
//    next_sine_value = (int) scaled_result + 2047.5; //Our output needs to be between 0 and 4095, inclusive

//    CAPTURE_TIME_0;
    MPY32CTL0 |= MPYM_1;
    MPYS = volume_range_data;
    OP2 = SinusoidArrayInt[current_array_index];

    // MPYS * OP2 * 2^12
//    OP2 = SinusoidArray[current_array_index];

    //The hardware multiplier requires 3 cycles after writing OP2 to complete the multiplication
    __delay_cycles(3);

    //read the result of the hardware multiplication
    // shift by 2 to get value desired, then shift by 12 to go back to original values
    l_amplitude = (((signed long) RES1 << 16) | (RES0)) >> 13;
    next_sine_value = l_amplitude + 2047;

//    scaled_result_int = (int) (volume_range_data * SinusoidArray[current_array_index]) >> 1;
//    next_sine_value = scaled_result_int + 2047;
    SAC_DATA_RGSTR = next_sine_value;

//    CAPTURE_TIME_1;
    // step forward in the array index

//    current_array_index += SINE_ARRAY_LENGTH/samps_per_cycle;
    if(samps_per_cycle == 64){
        current_array_index += 1;
    } else if (samps_per_cycle == 32){
        current_array_index += 2;
    } else if (samps_per_cycle == 16){
        current_array_index += 4;
    } else {
        current_array_index += 8;
    }
//    current_array_index = current_array_index % SINE_ARRAY_LENGTH;

    if(current_array_index >= 64){
        current_array_index -= 64;
    }

}

const int discretePeriod[36]={
1136,
1073,
1012,
956,
902,
851,
804,
758,
716,
676,
638,
602,
1136,
1073,
1012,
956,
902,
851,
804,
758,
716,
676,
638,
602,
1136,
1073,
1012,
956,
902,
851,
804,
758,
716,
676,
638,
602,
1136,
};
// The sine lookup table.
// I wanted to put this in constant definitions, but this caused a linking error because it would be defined multiple times.
// For lack a better way, I am putting it in here instead.

// bit shifted left by 12, (multiplied by 2^12)
const int powTable[36]={
2048,
2170,
2299,
2435,
2580,
2734,
2896,
3069,
3251,
3444,
3649,
3866,
4096,
4340,
4598,
4871,
5161,
5468,
5793,
6137,
6502,
6889,
7298,
7732,
8192,
8679,
9195,
9742,
10321,
10935,
11585,
12274,
13004,
13777,
14596,
15464,
16384,
};


const int SinusoidArrayInt[64] = {
4096,
4076,
4017,
3920,
3784,
3612,
3406,
3166,
2896,
2598,
2276,
1931,
1567,
1189,
799,
401,
0,
-401,
-799,
-1189,
-1567,
-1931,
-2276,
-2598,
-2896,
-3166,
-3406,
-3612,
-3784,
-3920,
-4017,
-4076,
-4096,
-4076,
-4017,
-3920,
-3784,
-3612,
-3406,
-3166,
-2896,
-2598,
-2276,
-1931,
-1567,
-1189,
-799,
-401,
0,
401,
799,
1189,
1567,
1931,
2276,
2598,
2896,
3166,
3406,
3612,
3784,
3920,
4017,
4076,
};
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
0,
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
