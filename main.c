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

#define PIN_TEST
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
#include <DAC.h>

// Function Prototypes
void init_CS();
void init_GlobalVariables(void);
void init_Constants(void);

// The sine lookup table.
// I wanted to put this in constant definitions, but this caused a linking error because it would be defined multiple times.
// For the life of me, I can't seem to declare this and find a good place to define it.
// Find a good home for this, some day.
const double SinusoidArray[64] = {
1,
0.995184727,
0.98078528,
0.956940336,
0.923879533,
0.881921264,
0.831469612,
0.773010453,
0.707106781,
0.634393284,
0.555570233,
0.471396737,
0.382683432,
0.290284677,
0.195090322,
0.09801714,
6.12574E-17,
-0.09801714,
-0.195090322,
-0.290284677,
-0.382683432,
-0.471396737,
-0.555570233,
-0.634393284,
-0.707106781,
-0.773010453,
-0.831469612,
-0.881921264,
-0.923879533,
-0.956940336,
-0.98078528,
-0.995184727,
-1,
-0.995184727,
-0.98078528,
-0.956940336,
-0.923879533,
-0.881921264,
-0.831469612,
-0.773010453,
-0.707106781,
-0.634393284,
-0.555570233,
-0.471396737,
-0.382683432,
-0.290284677,
-0.195090322,
-0.09801714,
-1.83772E-16,
0.09801714,
0.195090322,
0.290284677,
0.382683432,
0.471396737,
0.555570233,
0.634393284,
0.707106781,
0.773010453,
0.831469612,
0.881921264,
0.923879533,
0.956940336,
0.98078528,
0.995184727,
};


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    init_CS();
    init_Constants();

    init_GlobalVariables();
    init_HardwareSubsystems();

	//Initialize variables
	//Initialize hardware
	//Finally enable interrupts
    __enable_interrupt();

#ifdef PIN_TEST
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

// this function is taken from the example code - "Out of Box Experience" - for the MSP-EXP430FR2355'
// Initializes the clock system
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

void init_GlobalVariables(void) {
    volume_range_data = 2048;
    pitch_range_data = 2048;
    current_array_index = 0;

    DAC_period = calc_period(calc_freq(pitch_range_data));

    // the volume data is a 12-bit number, so we can cast it to double and multiply it by the sine_function
    // BUT that number will then vary from 0-4096, and that's too high because we're going to be adding the DC offset
    // SO we divide by 2 before casting it back to an int
    // the result will be a 12-bit number.
    double scaled_result;
    scaled_result = (double) volume_range_data * SinusoidArray[current_array_index] / 2;
    next_sine_value = (unsigned int) scaled_result + 2047.5; //Our output needs to be between 0 and 4095, inclusive

}

void init_Constants(void) {
    double min_freq_steps; //the number of half-steps away the minimum frequency is from the fixed note frequency
    double max_freq_steps; //the number of half-steps away the maximum frequency is from the fixed note frequency
    min_freq_steps = log(MIN_FREQ/FIXED_NOTE_FREQ)/log(FREQ_BASE);
    max_freq_steps = log(MAX_FREQ/FIXED_NOTE_FREQ)/log(FREQ_BASE);
    freq_step_range = (int) (max_freq_steps - min_freq_steps);

}

