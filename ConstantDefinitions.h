/*
 * ConstantDefinitions.h
 *
 *  Created on: Sep 24, 2020
 *      Author: harry
 */

#ifndef CONSTANTDEFINITIONS_H_
#define CONSTANTDEFINITIONS_H_

#define TICK_INTERVAL 100 // This might be a timer value etc.
#define MAX_ADC_VALUE 1023 // Also for adc
#define BUTTON_TRIES  5    // Part of some UI

//The following defines are for bit width definitions
//The underlying assumptions are"
//The capture timers are updated with a 1uS resolution
//A full bit time is 1 mS nominal
#define VALID_HALF_BIT_MIN    450
#define VALID_HALF_BIT_MAX    550
#define VALID_FULL_BIT_MIN    950
#define VALID_FULL_BIT_MAX   1050
#define MISSING_EDGE_TIMEOUT 1200


#endif /* CONSTANTDEFINITIONS_H_ */
