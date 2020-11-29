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

unsigned int buffer[16] = {0};
unsigned long total = 0;
unsigned int i = 0;

#pragma vector = TIMER1_B0_VECTOR
__interrupt void TimerB1ISR(void)
{
    // okay, so the ADC needs to:
    TURN_OFF_ADC;
    SET_ADC_INPUT_PITCH;
    TURN_ON_ADC;

    while (ADC_IS_BUSY);

    total = total - buffer[i];

    buffer[i] = ADC_DATA;

    total = total + buffer[i];
    i++;
    if (i > 15) i = 0;

    pitch_range_data = total >> 4;

    TURN_OFF_ADC;
    SET_ADC_INPUT_VOLUME;
    TURN_ON_ADC;

    while (ADC_IS_BUSY);

    TURN_OFF_SENSOR; // now that we've read the data, we can turn off the sensor.

    volume_range_data = ADC_DATA;

    TURN_ON_SENSOR; // now we want it to start reading the next sample, so turn on the sensor.
}

