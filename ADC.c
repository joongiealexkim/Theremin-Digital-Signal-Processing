/*
 * ADC.c
 *
 *  Created on: Oct 23, 2020
 *      Author: user
 */

#include "ADC.h"
#include <msp430.h>
#include <math.h>
#include "ConstantDefinitions.h"
#include "GlobalVariables.h"
#include "PortIODefs.h"

void ConfigureTimerB1(void)
{
    // Stop the Timer B1 and clear it.
    TB1CTL = (MC_0 | TBCLR);

    // set Capture/Compare Register 0 to Compare mode
    TB1CCTL0 &= ~(CAP);

    // Initialize TB1CCR0.
    TB1CCR0 = SENSOR_SAMPLE_PERIOD;

    // Configure the timer, including input clock select, input clock divide,
    // and starting the timer in up mode.
    TB1CTL |= (TBSSEL_2 | ID_3 | MC_1);

    // Enable TB1CCR0 Compare/Capture Interrupt Flag (CCIFG0)
    TB1CCTL0 |= CCIE;
}
int prev_pitch = 10000;
int prev_pitch_b = 10000;
#pragma vector = TIMER1_B0_VECTOR
// Interrupt service routine for CCIFG0
__interrupt void TimerB1ISR(void)
{

    // okay, so the ADC needs to:
    TURN_OFF_ADC;
    SET_ADC_INPUT_PITCH;
    TURN_ON_ADC;

    while (ADC_IS_BUSY);

    if (prev_pitch_b - ADC_DATA <= 115){
        pitch_range_data = prev_pitch;
    } else if (ADC_DATA - prev_pitch_b <= 115){
        pitch_range_data = prev_pitch;
    } else {
        pitch_range_data = ADC_DATA;
    }
    prev_pitch_b = ADC_DATA;
    prev_pitch = pitch_range_data;

    TURN_OFF_ADC;
    SET_ADC_INPUT_VOLUME;
    TURN_ON_ADC;

    while (ADC_IS_BUSY);

    TURN_OFF_SENSOR; // now that we've read the data, we can turn off the sensor.

    volume_range_data = ADC_DATA;

    TURN_ON_SENSOR; // now we want it to start reading the next sample, so turn on the sensor.
}
