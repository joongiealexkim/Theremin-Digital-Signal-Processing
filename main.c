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

//#define PIN_TEST
#define DEBUG_VERSION
#include <msp430.h>  //This is a standard include
#include "useful.h"
#include <math.h>
#include <stdio.h>
#include "IQmathLib.h"
/* External Functions
 *
 */
#include "PortIODefs.h"
#include "ConstantDefinitions.h"
#include "GlobalVariables.h"
#include "InitVariables.h"
#include "InitHardwareSubsystems.h"
#include <DAC.h>


#ifdef DEBUG_VERSION
#include <test_functions.h>
#endif


// Function Prototypes
void init_CS();
void init_GlobalVariables(void);
void init_Constants(void);


/**
 * main.c
 */
int main(void)
    {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

#ifdef DEBUG_VERSION // Simply sets p1.2 and p1.1 as outputs; the intention is to connect them to the ADC inputs to guarantee that the ADC is reading values
    P1DIR |= BIT2;
    P1OUT |= BIT2;
    P1SEL1 &= ~BIT2;
    P1SEL0 &= ~BIT2;

    P1DIR |= BIT1;
    P1OUT |= BIT1;
    P1SEL1 &= ~BIT1;
    P1SEL0 &= ~BIT1;
#endif

    //Initialize variables
    init_Constants();
    init_GlobalVariables();

    //Initialize hardware
    init_CS();
    init_HardwareSubsystems();
    ConfigureTimerB0(); // used for the DAC interrupt
    ConfigureTimerB1(); // used for the ADC interrupt

#ifdef DEBUG_VERSION
    ConfigureTimerB2();
#endif



	//Finally enable interrupts
    __enable_interrupt();

#ifdef PIN_TEST
    test_pin_all_square();
    test_pin_ADC();
    test_pin_DAC();
    test_pin_switch();
#endif

//    double scaled_result;
//    int scaled_result_int;
	while(1){
		//Main Loop
//	    if (SWITCH_IS_ON) DAC_period = calc_period(calc_freq(pitch_range_data, DISCRETE));
//	    else DAC_period = calc_period(calc_freq(pitch_range_data, CONTINUOUS));

	    // doesnt work in interrupt
//	    scaled_result_int = (int) (volume_range_data * SinusoidArray[current_array_index]) >> 1;
//	    next_sine_value = scaled_result_int + 2047;


	}
//	return 0;
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

    if (SWITCH_IS_ON) DAC_period = calc_period_full(pitch_range_data, DISCRETE);
    else DAC_period = calc_period_full(pitch_range_data, CONTINUOUS);

    // the volume data is a 12-bit number, so we can cast it to double and multiply it by the sine_function
    // BUT that number will then vary from 0-4096, and that's too high because we're going to be adding the DC offset
    // SO we divide by 2 before casting it back to an int
    // the result will be a 12-bit number.


}

void init_Constants(void) {
    min_freq_steps = log(MIN_FREQ*1.0/FIXED_NOTE_FREQ)/log(FREQ_BASE);
    max_freq_steps = log(MAX_FREQ*1.0/FIXED_NOTE_FREQ)/log(FREQ_BASE);
    freq_step_range = (long) (max_freq_steps - min_freq_steps);
    hardware_range = _IQ13(0.00024414); // (1/4096.0) workaround to get 1/4096
    freq_base_fixed = _IQ24(FREQ_BASE);// rename
    fixed_note_freq_fixed = _IQ16(FIXED_NOTE_FREQ); // rename
    freq_base_log = _IQ24log(freq_base_fixed);

}

