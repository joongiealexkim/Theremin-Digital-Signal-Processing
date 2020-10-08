/************************************************************************************************************************************
 * Currently just a skeleton program that does nothing. Thanks to Professor Powell for the skeleton.
 * 9/24/2020
 * Author: Alex Kim
 * The purpose of this is to perform digital signal processing.
 * Essentially, this microcontroller will be functioning as a synthesizer,
 * using the range data from the optical sensors to generate specific sinusoidal waves
 * and output them using the built in DAC.
************************************************************************************************************************************/
//No function should go beyond 1 readable page.
//Check according to MISRA

#define DEBUG_VERSION
#include <msp430.h>  //This is a standard include
#include "useful.h"
#include "ConstantDefinitions.h"
#include "PortIODefs.h"
#include "GlobalVariables.h"

#ifdef DEBUG_VERSION
#include "test_pins.h"
#endif

/* External Functions
 *
 */
#include "InitVariables.h"
#include "InitHardwareSubsystems.h"

// Function Prototype
void init_CS();

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    init_CS();
	//Initialize variables
	//Initialize hardware
	//Finally enable interrupts

#ifdef DEBUG_VERSION
    test_pin_all_square();
    test_pin_ADC();
    test_pin_DAC();
    test_pin_switch();
#endif

	while(1){
		//Main Loop

	}
	return 0;
}

// this function is taken from the example code - "Out of Box Experience" - for the MSP-EXP430FR2355
void init_CS(void) {
    // Configure two FRAM waitstate as required by the device datasheet for MCLK
    // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_2 ;

    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    __bis_SR_register(SCG0);                     // disable FLL
    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
    CSCTL0 = 0;                                  // clear DCO and MOD registers
    CSCTL1 = DCORSEL_5;                          // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;                       // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;   // set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
                                                 // default DCOCLKDIV as MCLK and SMCLK source

    P3DIR |= BIT4;
    P3SEL0 |= BIT4;
    P3SEL1 &= ~BIT4;
}
