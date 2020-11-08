/*
 * test_functions.h
 *
 *  Created on: Oct 2, 2020
 *      Author: user
 *
 *  Contains functions that test the functionality of all port pins that will be used for the system.
 *  NOTE: these functions require a debugger to be used! They all contain infinite loops that can only be broken
 *  by modifying the loop variable with a debugger.
 */
#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

//these macros are used, along with ConfigureTimerB2, to capture the time at which they are called.
//They are all on the 16 MHz clock!
#define CAPTURE_TIME_0      TB2CCTL0 ^= CCIS0
#define CAPTURE_TIME_1      TB2CCTL1 ^= CCIS0

; //er... i dont know why, but my code won't compile on unless I put this semicolon here. Using Code Composer Studio 10.1.1

/*
 * The first function does this by setting all used pins to output mode, then creating a square wave at each of them
 * the square wave has a period of 0.5 seconds (~0.25 second high, ~0.25 second low), with a high value of 3.3V and a low value of 0 V.
 * (that is, a high of VCC and low of VDD)
 */
void test_pin_all_square();

/*
 * The second function ties the ADC inputs to the two LEDS
 * That is, applying a voltage to pin5.0 will make LED1 light up.
 * Similarly, applying a voltage to pin5.1 will make LED2 light up.
 */
void test_pin_ADC();

/*
 * The third function has the DAC output a constant value (to pin 3.1), instead of putting it on the port pins directly
 * This constant value should be equal to Vcc/2 (~1.65V assuming VCC = 3.3V)
 */
void test_pin_DAC();

/*
 * The fourth function ties the switch input (p1.3) to LED1.
 * That is, applying a high voltage to the switch input will cause LED1 to light up. A low voltage will turn it off.
 */
void test_pin_switch();

/*
 * This is to test the timing of computation.
 * Several are additionally defined in order to retrieve the data.
 */
void ConfigureTimerB2();

#endif /* TEST_FUNCTIONS_H_ */
