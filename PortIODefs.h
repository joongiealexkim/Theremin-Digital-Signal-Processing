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
#define SWITCH_DIR_PORT                 P1DIR
#define SWITCH_PORT_SELECT_1            P1SEL1
#define SWITCH_PORT_SELECT_0            P1SEL0
#define SWITCH_OUTPUT                   P1OUT
#define SWITCH_INPUT
#define SWITCH_RESISTOR                 P1REN

#define SWITCH_PIN                      BIT3

#define SET_SWITCH_PIN_AS_INPUT         SWITCH_DIR_PORT &= ~(SWITCH_PIN)
#define SET_SWITCH_PIN_AS_GPIO          SWITCH_PORT_SELECT_1 &= ~(SWITCH_PIN);SWITCH_PORT_SELECT_0 &= ~(SWITCH_PIN)
#define ENABLE_SWITCH_PULL_RESISTOR     SWITCH_RESISTOR |= SWITCH_PIN
#define SWITCH_PULL_DOWN_RESISTOR       SWITCH_OUTPUT &= ~SWITCH_PIN

#define SWITCH_IS_ON                    (P1IN & BIT3) == BIT3


/**
 * Sensor GPIO port
 * This port will be for activating the Pitch/Volume Sensors
 * And will be a simple GPIO on pin 1.2
 */
#define SENSOR_DIR_PORT                 P1DIR
#define SENSOR_PORT_SELECT_1            P1SEL1
#define SENSOR_PORT_SELECT_0            P1SEL0
#define SENSOR_OUTPUT                   P1OUT

#define SENSOR_PIN                      BIT2

#define SET_SENSOR_PIN_AS_OUTPUT        SENSOR_DIR_PORT |= (SENSOR_PIN)
#define SET_SENSOR_PIN_AS_GPIO          SENSOR_PORT_SELECT_1 &= ~(SENSOR_PIN);SENSOR_PORT_SELECT_0 &= ~(SENSOR_PIN)
#define TURN_ON_SENSOR                  SENSOR_OUTPUT |= SENSOR_PIN
#define TURN_OFF_SENSOR                 SENSOR_OUTPUT &= ~(SENSOR_PIN)


#endif /* PORTIODEFS_H_ */
