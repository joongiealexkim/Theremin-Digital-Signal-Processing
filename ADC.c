/*
 * ADC.c
 *
 *  Created on: Oct 23, 2020
 *      Author: user
 */

#include "ADC.h"

void ConfigureTimerB1(void)
{
    // Stop the Timer B0 and clear it.
    TB1CTL = (MC_0 | TBCLR);

    // Configure the timer, including input clock select, input clock divide,
    // and starting the timer in up mode.
    TB1CTL |= (TBSSEL_2 | ID_0 | MC_1);
    // Initialize TB0CCR0.
    TB1CCR0 = SENSOR_SAMPLE_PERIOD;

    // set Timer B0 to Compare mode
    TB1CCTL0 &= ~(CAP);
    // Enable TB0CCR0 Compare/Capture Interrupt Flag (CCIFG0)
    TB1CCTL0 |= CCIE;
}

#pragma vector = TIMER1_B0_VECTOR
// Interrupt service routine for CCIFG0
__interrupt void TimerB1ISR(void)
{
    // okay, so the ADC needs to:
    TURN_OFF_ADC;
    SET_ADC_INPUT_PITCH;
    TURN_ON_ADC;

    while (ADC_IS_BUSY);

    pitch_range_data = ADC_DATA;

    TURN_OFF_ADC;
    SET_ADC_INPUT_VOLUME;
    TURN_ON_ADC;

    while (ADC_IS_BUSY);

    volume_range_data = ADC_DATA;

    // now we can turn off the sensors, but...
    // then we gotta turn them on again to begin sampling once more.

    // i got an idea: send a rising edge to the sensors at the end fo the interrupt, then read them and unset them at the beginning of the interrupt!
    // i'm a genius!


}
