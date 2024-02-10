/*
 * BluetoothModule.h
 *
 *  Created on: Feb 9, 2024
 *      Author: Tuan Tran
 */
#include "msp.h"
#ifndef BLUETOOTHMODULE_H_
#define BLUETOOTHMODULE_H_
/* Define Pins and Ports */
#define BLUETOOTH_PORT  P3
#define RX_PIN         BIT2 //RX on MSP; goes to TX on bluetooth
#define TX_PIN         BIT3 //RX on MSP; goes to RX on bluetooth
#define BLUETOOTH_PINS RX_PIN|TX_PIN

extern void InitBluetooth();
//TODO: Document
extern void SendCharArrayBluetooth(char *Buffer);
//TODO: Document
extern char GetCharBluetooth();
//TODO: Document

#endif /* BLUETOOTHMODULE_H_ */
