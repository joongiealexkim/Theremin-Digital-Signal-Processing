/*
 * PortIODefs.h
 *
 *  Created on: Sep 24, 2020
 *      Author: harry
 */

#ifndef PORTIODEFS_H_
#define PORTIODEFS_H_

#define SWITCH_PORT P1IN
#define START_SWITCH_BIT  0x01    //Switch on Port 1 Bit 0


//Constant define section:
#define BITS_IN_TRANSMISSION  32
#define TRANSMIT_PIN          B8_5(P1OUT)
#define TRANSMIT_PORT         P1
#define TRANSMIT_PORT_DIR     P1DIR
#define TRANSMIT_BIT          5

#define TX_RCV_MODE			  B8_6(P1OUT)
#define TX_RCV_PORT_DIR       P1DIR
#define TX_RCV_BIT            6

//Establish bits and registers for receiver pins
#define RCV_CAPTURE_BIT0      0
#define RCV_CAPTURE_BIT1      1
#define RCV_PORT_SEL          P2SEL
#define RCV_PORT_SEL2         P2SEL2

#define DEBUG_LED             B8_0(P1OUT)
#define LED_PORT_DIR          P1DIR
#define LED_BIT               0
#define LED_ON                HIGH
#define LED_OFF               LOW
// Note that the tx_rcv mode should be high to receive.



#endif /* PORTIODEFS_H_ */
