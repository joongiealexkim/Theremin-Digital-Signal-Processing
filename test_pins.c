/*
 * test_pins.c
 *
 *  Created on: Oct 2, 2020
 *      Author: user
 */

#include <msp430.h>
#include "test_pins.h"

// The first function does this by setting all used pins to output mode, then creating a square wave at each of them
// the square wave has a period of two seconds (~1 second high, ~1 second low), with a high value of 3.3V and a low value of 0 V.
// (that is, a high of VCC and low of VDD)
void test_pin_all_square() {
    // initialize port pins
    // I will be using pins p5.0, p5.1, p3.1, p1.3, p1.2, p2.0, p4.0

    // set p5.0 and p5.1 as output
    P5DIR |= (BIT0 | BIT1);
    //set to general purpose I/O
    P5SEL1 &= ~(BIT0 | BIT1);
    P5SEL0 &= ~(BIT0 | BIT1);

    // set p4.0 as output
    P4DIR |= (BIT0);
    P4SEL1 &= ~(BIT0);
    P4SEL0 &= ~(BIT0);

    // set p3.1 as output
    P3DIR |= (BIT1);
    //set to general purpose I/O
    P3SEL1 &= ~(BIT1);
    P3SEL0 &= ~(BIT1);

    // set p2.0 as output
    P2DIR |= (BIT0);
    P2SEL1 &= ~(BIT0);
    P2SEL0 &= ~(BIT0);

    // set p1.2 and p1.3 as output
    P1DIR |= (BIT2 | BIT3);
    P1SEL1 &= ~(BIT2 | BIT3);
    P1SEL0 &= ~(BIT2 | BIT3);


    // this variable is meant to ensure that the loop continues without end
    // until it is manually changed using the debug mode.
    int debug_loop;
    debug_loop = 1;

    while (debug_loop == 1) {
        // sleep for 16,000,000 clock cycles.
        // Assuming a 16 MHz clock, 1 second.
        _delay_cycles(1000000);

        // set the pin outputs to "high"
        P5OUT |= (BIT0 | BIT1);
        P4OUT |= (BIT0);
        P3OUT |= (BIT1);
        P2OUT |= (BIT0);
        P1OUT |= (BIT2 | BIT3);

        _delay_cycles(1000000);

        // set the pin outputs to "low"
        P5OUT &= ~(BIT0 | BIT1);
        P4OUT &= ~(BIT0);
        P3OUT &= ~(BIT1);
        P2OUT &= ~(BIT0);
        P1OUT &= ~(BIT2 | BIT3);

        _nop();
    }
}

// The second function ties the ADC inputs to the two LEDS
// That is, applying a voltage to pin5.0 will make LED1 light up.
// Similarly, applying a voltage to pin5.1 will make LED2 light up.
void test_pin_ADC() {
    // Set P5.0 and P5.1 as ADC input
    P5SEL1 &= ~(BIT0 | BIT1);
    P5SEL0 &= ~(BIT0 | BIT1);

    //first, ensure that ADC is off
    ADCCTL0 &= ~(ADCENC);
    ADCCTL0 &= ~(ADCON);
    // default value for sample-and-hold time is 1h->8 cycles on a 12-bit ADC
    // set multiple sample-and-conversion
    ADCCTL0 &= ~(ADCMSC);

    // set sample-and-hold source to ADCSC bit
    ADCCTL1 &= ~(ADCSHS1 | ADCSHS0);
    // sample input signal is not inverted
    ADCCTL1 &= ~(ADCISSH);
    // clock divide set to 1
    ADCCTL1 &= ~(ADCDIV2 | ADCDIV1 | ADCDIV0);
    // set clock source to MODCLK
    ADCCTL1 &= ~(ADCSSEL1 | ADCSSEL0);
    //set to single-channel single-conversion mode
    ADCCTL1 &= ~(ADCCONSEQ1 | ADCCONSEQ0);

    // Set predivider to 1
    ADCCTL2 &= ~(ADCPDIV1 | ADCPDIV0);
    // 12 bit resolution
    ADCCTL2 |=  ADCRES1;
    ADCCTL2 &= ~ADCRES0;
    // set data read-back format to unsigned
    ADCCTL2 &= ~(ADCDF);
    // set sampling rate to 200 ksps
    ADCCTL2 &= ~(ADCSR);


    // set reference voltage to AVCC and AVSS
    ADCMCTL0 &= ~(ADCSREF2 | ADCSREF1 | ADCSREF0);

    // set P1.0 and P1.6 as output
    P1DIR |= (BIT0 | BIT6);
    //set to general purpose I/O
    P1SEL1 &= ~(BIT0 | BIT6);
    P1SEL0 &= ~(BIT0 | BIT6);
    // initialize outputs to zero
    P1OUT &= ~(BIT0 | BIT6);


    int debug_loop;
    debug_loop = 1;
    int A0_input, A1_input;

    while (debug_loop == 1) {
        // set ADC input to pin p5.0, input A0
        ADCMCTL0 &= ~(ADCINCH3 | ADCINCH2 | ADCINCH1 | ADCINCH0);
        // activate ADC
        ADCCTL0 |= (ADCON);
        ADCCTL0 |= (ADCSC | ADCENC);

        // while the "busy" bit in ADCCTL1 is set to 1; that is, while a conversion is active
        while (ADCCTL1 & ADCBUSY) {
            // wait for it to finish and become not busy
        }
        // read ADC output
        A0_input = ADCMEM0;
        // we will say that the input was HIGH if the ADC value was greater than 2048, half the max value
        // activate the green LED if the input was high, deactivate it if the input was low
        if (A0_input > 2048) P1OUT |= (BIT0);
        else P1OUT &= ~(BIT0);

        //deactivate ADC
        ADCCTL0 &= ~(ADCENC);
        ADCCTL0 &= ~(ADCON);
        // set ADC input to pin 5.1, input A1
        ADCMCTL0 &= ~(ADCINCH3 | ADCINCH2 | ADCINCH1);
        ADCMCTL0 |= (ADCINCH0);
        //activate ADC
        ADCCTL0 |= (ADCON);
        ADCCTL0 |= (ADCSC | ADCENC);
        // while a conversion is active
        while (ADCCTL1 & ADCBUSY) {
            // wait for it to finish
        }
        //read ADC output
        A1_input = ADCMEM0;

        // active the red LED if the input was high, deactivate it if the input was low
        if (A1_input > 2048) P1OUT |= (BIT6);
        else P1OUT &= ~(BIT6);

        // there's no reason to check this every clock cycle.
        _delay_cycles(10000);

        _nop();
    }



}

// The third function has the DAC output a square wave (to pin 3.1), instead of putting it on the port pins directly
void test_pin_DAC() {
    // configure output port pin p3.1
    P3SEL1 |= BIT1;
    P3SEL0 |= BIT1;

    // disable SAC
    SAC0OA &= ~(SACEN);
    // Set SAC to buffer mode
    SAC0OA &= ~(NSEL1);
    SAC0OA |= (NSEL0);

    SAC0OA &= ~(PSEL1);
    SAC0OA |= (PSEL0);

    SAC0PGA &= ~(MSEL1);
    SAC0PGA |= (MSEL0);

    //disable DAC
    SAC0DAC &= ~(DACEN);
    // set to primary reference voltage
    SAC0DAC &= ~(DACSREF);
    // set to "DAC latch loads when DACDAT is written"
    SAC0DAC &= ~(DACLSEL1 | DACLSEL0);

    int debug_loop;
    debug_loop = 1;

    //active SAC and DAC
    SAC0OA |= (SACEN);
    SAC0DAC |= (DACEN);
    while (debug_loop == 1) {
        SAC0DAT = 4095;
        _delay_cycles(1000000);
        SAC0DAT = 0;
        _delay_cycles(1000000);

        _nop();
    }


}

// The fourth function ties the switch input to LED1
void test_pin_switch() {
    // set p1.3 as input
    P1DIR &= ~(BIT3);
    //set to general purpose I/O
    P1SEL1 &= ~(BIT3);
    P1SEL0 &= ~(BIT3);
    // enable pull-up/pull-down resistors for P3.2
    P1REN |= BIT3;
    // enable pull-down resistor for P3.2
    P1OUT &= ~BIT3;

    // set P1.0 as output
    P1DIR |= (BIT0);
    // initialize output to zero
    P1OUT &= ~(BIT0);
    //set to general purpose I/O
    P1SEL1 &= ~(BIT0);
    P1SEL0 &= ~(BIT0);

    // this variable is meant to ensure that the loop continues without end
    // until it is manually changed using the debug mode.
    int debug_loop;
    debug_loop = 1;

    while (debug_loop == 1) {
        // if we read a high input at p1.3,
        if ((P1IN & BIT3) == 1) {
            // then output a signal at P1.0, which is connected to LED1.
            P1OUT |= (BIT0);
        } else {
            // otherwise, output 0 at P1.0
            P1OUT &= ~(BIT0);
        }
        // there's no reason to check this every clock cycle.
        _delay_cycles(10000);
        _nop();
    }
}
