/*
 * test_pins.c
 *
 *  Created on: Oct 2, 2020
 *      Author: user
 */

#include <msp430.h>
#include "test_pins.h"


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
        // sleep for 4,000,000 clock cycles.
        // Assuming a 16 MHz clock, 0.25 seconds.
        _delay_cycles(4000000);

        // set the pin outputs to "high"
        P5OUT |= (BIT0 | BIT1);
        P4OUT |= (BIT0);
        P3OUT |= (BIT1);
        P2OUT |= (BIT0);
        P1OUT |= (BIT2 | BIT3);

        _delay_cycles(4000000);

        // set the pin outputs to "low"
        P5OUT &= ~(BIT0 | BIT1);
        P4OUT &= ~(BIT0);
        P3OUT &= ~(BIT1);
        P2OUT &= ~(BIT0);
        P1OUT &= ~(BIT2 | BIT3);

        _nop();
    }
}

void test_pin_ADC() {
    // Set P5.0 and P5.1 as ADC input
    P5SEL1 |= (BIT0 | BIT1);
    P5SEL0 |= (BIT0 | BIT1);

    //first, ensure that ADC is off
    ADCCTL0 &= ~(ADCENC);
    ADCCTL0 &= ~(ADCON);
    ADCCTL0 &= ~(ADCSC);
    // Initialize interrupt vectors
    ADCIE &= 0x0000;
    ADCIFG &= 0x0000;

    // default value for sample-and-hold time is 1h->8 cycles on a 12-bit ADC
    // disable multiple sample-and-conversion
    ADCCTL0 &= ~(ADCMSC);

    // set sample-and-hold source to ADCSC bit
    ADCCTL1 &= ~(ADCSHS1 | ADCSHS0);
    // sample input signal is not inverted
    ADCCTL1 &= ~(ADCISSH);
    // clock divide set to 4
    ADCCTL1 &= ~(ADCDIV2);
    ADCCTL1 |= (ADCDIV1 | ADCDIV0);
    // set clock source to SMCLK - the 16 MHz clock
    ADCCTL1 |= (ADCSSEL1 | ADCSSEL0);
    //set to single-channel single-conversion mode
    ADCCTL1 &= ~(ADCCONSEQ1 | ADCCONSEQ0);
    // set to pulse sample mode
    ADCCTL1 |= ADCSHP;

    // Set predivider to 4
    ADCCTL2 |= ADCPDIV0;
    ADCCTL2 &= ~(ADCPDIV1);
    // 12 bit resolution
    ADCCTL2 |=  ADCRES1;
    ADCCTL2 &= ~ADCRES0;
    // set data read-back format to unsigned
    ADCCTL2 &= ~(ADCDF);
    // set sampling rate to 200 ksps
    ADCCTL2 &= ~(ADCSR);


    // set reference voltage to AVCC and AVSS
    ADCMCTL0 &= ~(ADCSREF2 | ADCSREF1 | ADCSREF0);

    // set P1.0 and P6.6 as output
    P1DIR |= (BIT0);
    P6DIR |= (BIT6);
    //set to general purpose I/O
    P1SEL1 &= ~(BIT0);
    P1SEL0 &= ~(BIT0);
    P6SEL1 &= ~(BIT6);
    P6SEL0 &= ~(BIT0);
    // initialize outputs to zero
    P1OUT &= ~(BIT0 | BIT6);


    int debug_loop;
    debug_loop = 1;
    int A8_input, A9_input;

    while (debug_loop == 1) {
        //deactivate ADC
        ADCCTL0 &= ~(ADCENC);
        ADCCTL0 &= ~(ADCON);

        // set ADC input to pin p5.0, input A8
        ADCMCTL0 &= ~(ADCINCH2 | ADCINCH1 | ADCINCH0);
        ADCMCTL0 |= (ADCINCH3);
        // activate ADC
        ADCCTL0 |= (ADCON);
        ADCCTL0 |= (ADCSC | ADCENC);

        // while the "busy" bit in ADCCTL1 is set to 1; that is, while a conversion is active
        while (ADCCTL1 & ADCBUSY) {
            // wait for it to finish and become not busy
        }
        // read ADC output
        A8_input = ADCMEM0;
        // we will say that the input was HIGH if the ADC value was greater than 2048, half the max value
        // activate the green LED if the input was high, deactivate it if the input was low
        if (A8_input > 1048) P1OUT |= (BIT0);
        else P1OUT &= ~(BIT0);

        //deactivate ADC
        ADCCTL0 &= ~(ADCENC);
        ADCCTL0 &= ~(ADCON);
        // set ADC input to pin 5.1, input A9
        ADCMCTL0 &= ~(ADCINCH2 | ADCINCH1);
        ADCMCTL0 |= (ADCINCH3 | ADCINCH0);
        //activate ADC
        ADCCTL0 |= (ADCON);
        ADCCTL0 |= (ADCSC | ADCENC);
        // while a conversion is active
        while (ADCCTL1 & ADCBUSY) {
            // wait for it to finish
        }
        //read ADC output
        A9_input = ADCMEM0;

        // active the red LED if the input was high, deactivate it if the input was low
        if (A9_input > 1048) P6OUT |= (BIT6);
        else P6OUT &= ~(BIT6);

        // there's no reason to check this every clock cycle.
        _delay_cycles(10000);

        _nop();
    }



}

void test_pin_DAC() {
    // configure output port pin p3.1
    P3SEL1 |= BIT1;
    P3SEL0 |= BIT1;

    // disable SAC
    SAC2OA &= ~(SACEN);
    // Set SAC to buffer mode
    SAC2OA &= ~(NSEL1);
    SAC2OA |= (NSEL0);

    // this selects the DAC as the non-inverting input
    SAC2OA &= ~(PSEL1);
    SAC2OA |= (PSEL0);

    //enable both op-amp inputs
    SAC2OA |= PMUXEN | NMUXEN;

    SAC2PGA &= ~(MSEL1);
    SAC2PGA |= (MSEL0);

    //disable DAC
    SAC2DAC &= ~(DACEN);
    // set to primary reference voltage
    SAC2DAC &= ~(DACSREF);
    // set to "DAC latch loads when DACDAT is written"
    SAC2DAC &= ~(DACLSEL1 | DACLSEL0);

    int debug_loop;
    debug_loop = 1;

    //activate the SAC Op-Amp, SAC, and DAC
    SAC2OA |= (OAEN);
    SAC2OA |= (SACEN);
    SAC2DAC |= (DACEN);
    while (debug_loop == 1) {
        SAC2DAT = 2048;
        _delay_cycles(4000000);
        _nop();
    }


}

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
        if ((P1IN & BIT3) == BIT3) {
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
