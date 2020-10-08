/*
 * test_pins.h
 *
 *  Created on: Oct 2, 2020
 *      Author: user
 *
 *  Contains functions that test the functionality of all port pins that will be used for the system.

 *
 *  Then, for all input pins, it links their input to an LED
 */
#ifndef TEST_PINS_H
#define TEST_PINS_H

; //er... i dont know why, but my code won't compile on unless I put this semicolon here. Using Code Composer Studio 10.1.1

/*
 * The first function does this by setting all used pins to output mode, then creating a square wave at each of them
 * the square wave has a period of two seconds (1 second high, 1 second low), with a high value of 1 V and a low value of 0 V.
 *(that is, a high of VCC and low of VDD)
 *
 */
void test_pin_all_square();

// The second function ties the ADC inputs to the two LEDS
// That is, applying a voltage to pin5.0 will make LED1 light up.
// Similarly, applying a voltage to pin5.1 will make LED2 light up.
void test_pin_ADC();

// The third function has the DAC output a square wave, instead of putting it on the port pins directly
void test_pin_DAC();

// The fourth function ties the switch input to LED1
void test_pin_switch();

#endif /* TEST_PINS_H_ */
