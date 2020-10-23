/*
 * InitHardwareSubsystems.c
 *
 *  Created on: Oct 16, 2020
 *      Author: user
 */
#include "InitHardwareSubsystems.h"
#include "PortIODefs.h"

void init_HardwareSubsystems() {
    init_ADC();
    init_DAC();
    init_Switch();
}

void init_ADC() {
    //configure the input pins
    CONFIGURE_PITCH_INPUT_PIN;
    CONFIGURE_VOLUME_INPUT_PIN;

    //ADC can only be configured while off
    TURN_OFF_ADC;

    // keep default value for sample-and-hold time
    // default value for sample-and-hold time is 8 cycles on a 12-bit ADC

    DISABLE_MULTI_SAMP_CONVERSION;

    SET_ADCSC_BIT_AS_SAMP_HOLD_SRC;
    SAMP_SIGNAL_NON_INVERTED;
    DIVIDE_ADC_CLK_BY_4;
    SET_CLK_SRC_AS_SMCLK;
    SINGLE_CHAN_SING_CONV_MODE;
    PULSE_SAMPLE_MODE;

    PREDIVIDE_ADC_CLK_BY_4;
    SET_RESOLUTION_12_BIT;
    SET_DATA_FORMAT_UNSIGNED;
    SET_SAMPLING_RATE_200_KSPS;

    SET_REF_VOLTAGE_AVCC_AVSS;
}

void init_DAC() {
    CONFIGURE_DAC_OUTPUT_PIN;

    DISABLE_OPAMP;
    OPAMP_NON_INVERTED_BUFFER_MODE;
    ENABLE_OPAMP_INPUTS;

    DISABLE_DAC;
    DAC_SET_PRIMARY_REF_VOLTAGE;
    DAC_LATCH_LOAD_ON_WRITE;
}

void init_Switch() {
    SET_SWITCH_PIN_AS_INPUT;
    SET_SWITCH_PIN_AS_GPIO;
    ENABLE_SWITCH_PULL_RESISTOR;
    SELECT_PULL_DOWN_RESISTOR;
}
