/*
 * PortIODefs.h
 *
 *  Created on: Sep 24, 2020
 *      Author: harry
 */
#include <msp430.h>
#ifndef PORTIODEFS_H_
#define PORTIODEFS_H_

/**
 * ADC Ports
 * The two ADC input pins that will be used are p5.0 and p5.1.
 * These correspond to input A8 and A9 respectively.
 */
#define ADC_PITCH_PORT_SELECT_1         P5SEL1
#define ADC_PITCH_PORT_SELECT_0         P5SEL0
#define ADC_VOLUME_PORT_SELECT_1        P5SEL1
#define ADC_VOLUME_PORT_SELECT_0        P5SEL0

// Pitch input at pin 5.0, Volume input at pin 5.1
#define ADC_PITCH_INPUT_PIN             BIT0
#define ADC_VOLUME_INPUT_PIN            BIT1

// Configures the selected pins as ADC inputs
#define CONFIGURE_PITCH_INPUT_PIN       ADC_PITCH_PORT_SELECT_1 |= ADC_PITCH_INPUT_PIN; ADC_PITCH_PORT_SELECT_0 |= ADC_PITCH_INPUT_PIN
#define CONFIGURE_VOLUME_INPUT_PIN      ADC_VOLUME_PORT_SELECT_1 |= ADC_VOLUME_INPUT_PIN; ADC_VOLUME_PORT_SELECT_0 |= ADC_VOLUME_INPUT_PIN

/**
 * DAC Port
 * The DAC output pin will be p3.1.
 * This corresponds to the DAC on SAC2.
 */
#define DAC_OUTPUT_PORT_SELECT_1        P3SEL1
#define DAC_OUTPUT_PORT_SELECT_0        P3SEL0

#define DAC_OUTPUT_PIN                  BIT1

// Configures the selected pin as DAC output
#define CONFIGURE_DAC_OUTPUT_PIN        DAC_OUTPUT_PORT_SELECT_1 |= DAC_OUTPUT_PIN; DAC_OUTPUT_PORT_SELECT_0 |= DAC_OUTPUT_PIN

/**
 * Switch port
 * This port will be for selecting the operation mode
 * And will be a simple GPIO on pin 1.3
 */
#define SET_SWITCH_PIN_AS_INPUT         P1DIR &= ~(BIT3)
#define SET_SWITCH_PIN_AS_GPIO          P1SEL1 &= ~(BIT3);P1SEL0 &= ~(BIT3)


#endif /* PORTIODEFS_H_ */
