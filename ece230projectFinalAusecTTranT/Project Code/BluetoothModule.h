/*
 * BluetoothModule.h
 *
 *  Created on: Feb 9, 2024
 *      Author: Tuan Tran
 */
#ifndef BLUETOOTHMODULE_H_
#define BLUETOOTHMODULE_H_
#include "msp.h"
#include "main.h"
/* Define Pins and Ports */
#define BLUETOOTH_PORT  P3
#define RX_PIN         BIT2 //TX on MSP; goes to TX on bluetooth
#define TX_PIN         BIT3 //RX on MSP; goes to RX on bluetooth
#define BLUETOOTH_PINS RX_PIN|TX_PIN

extern void InitBluetooth();
//TODO: Document
extern void SendCharArrayBluetooth(char *Buffer);
//TODO: Document
extern void SetBluetoothFlags();
//TODO: Document
extern void SendUserPrompt();

extern void SendAttemptsBluetooth(int currentTries);

extern void SendStatusBluetooth(bool safeOpenFlag);

extern void SendAccessLogBluetooth(int AccessLog[][], int RTCIndex);

extern void HandleRecievedValue();

extern void SendUnlockStatusBluetooth(bool unlockedFlag);

extern bool GetSendAttemptsFlag();

extern bool GetSendDoorStatusFlag();

extern bool GetSendAccessLogFlag();

extern bool GetSendUnlockFlag();


#endif /* BLUETOOTHMODULE_H_ */
