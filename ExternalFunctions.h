/*
 * ExternalFunctions.h
 *
 *  Created on: Sep 24, 2020
 *      Author: harry
 */

#ifndef EXTERNALFUNCTIONS_H_
#define EXTERNALFUNCTIONS_H_

/**********************************************************************************************************************************************************************
 * Function name
 * purpose
 * input parameters - name and type
 * definition of return values
 * Where this function is called - multiple callers
 * **********************************************************************************************************************************************************************
 */
int ResetGlobalButtonpresses(unsigned int maxallowable, unsigned int presses){
	//local variable
	if (presses > maxallowable){
		presses = 0;
		return 1 ;
	}
	else return 0 ;
}

#endif /* EXTERNALFUNCTIONS_H_ */
