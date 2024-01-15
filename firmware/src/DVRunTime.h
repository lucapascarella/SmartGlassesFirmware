/*
    (c) [2022] Microchip Technology Inc. and its subsidiaries.
 
    Subject to your compliance with these terms, you may use Microchip
    software and any derivatives exclusively with Microchip products.
    You are responsible for complying with 3rd party license terms 
    applicable to your use of 3rd party software (including open source 
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.?
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.
*/

//------------------------------------------------------------------------------
#ifndef DVRUNTIME_H
#define	DVRUNTIME_H

// Constants that should be defined by the user
#define DV_ID_FW				(uint16_t)0x0105	// ID for Fw revision, returned in Ping response
#define DVPMT_NUM				8					// Number of Dynamic Variables Pointers in the Monitor Table 	
#define DV_STREAM_TIME			20					// Streaming time interval (time dependent on main loop tick)
#define DV_RX_CMD_TIMEOUT		400					// Cmd timeout (time dependent on main loop tick)
#define DV_INTERRUPT_DRIVEN_TX	0					// if 1 interrupt driven Uart TX (Rx always interrupt driven)

#define DV_DOB_SIZE			(DVPMT_NUM*4)+4		// Used just if INTERRUPT driven TX is enabled (*4 considering that all the variables are 32bit size)
												// Dynamic Output Buffer size containing all the variables to be Transmitted including SOF-16bitCounter-EOF
#define DV_START_FRAME		0x03				// Start Frame Byte
#define DV_END_FRAME		0xFC				// End Frame Byte
#define DV_RX_CMD_MIN_SIZE	3					// Minimun number of byte in a RX command


//------------------------------------------------------------------------------
// TYPES

// Command IDs
enum { 
    UDVPMT=0,   // Update Dynamic Variable Pointer Memory Table
    UVVAL=1,    // Update Variable Value
    USTICK=2,   // Update Streaming Tick
    SOFF=3,     // Streaming Off
    SON=4,      // Streaming On
    OSR=5,      // One Shot Read
    PING=6      // Ping
};

typedef struct DVPMTs {
	__ram uint8_t *address;
    uint8_t size;
} DVPMT_t;

typedef struct DVPMTaddrs {
    uint8_t size;
    __ram uint8_t *address;
} DVPMTaddr_t;

typedef struct DVStreamUpdates {
    uint8_t SOF;
    uint8_t command;
    uint8_t size;
    DVPMTaddr_t DVPMT[DVPMT_NUM];
    uint8_t EOFrm;
} DVPMT_Update_t;

typedef struct DVVarUpdates {
    uint8_t SOF;
    uint8_t command;
    uint8_t size;
    __ram uint8_t * address;
	uint8_t dataBuff[5];
} DVVarUpdate_t;

typedef struct DVIntervalUpdates {
    uint8_t SOF;
    uint8_t command;
    uint16_t period;
    uint8_t EOFrm;
} DVStreamTickUpdate_t;

typedef struct {
    uint8_t SOF;
    uint8_t command;
    uint8_t data;						
    uint8_t EOFrm;
} DVgenericCmd_t;

typedef struct DVs {
    uint8_t Buffer[DV_DOB_SIZE];
    uint8_t Index;
    uint8_t Length;
	uint8_t TxIndex;
} DV_OutBuff_t;


//------------------------------------------------------------------------------
// FUNCTIONS
void DVRT_Process(void);
void DVRT_Initialize(void);
void DVRT_SendData(void);
void DVRT_RxCommand(void);
void UART_DVRT_WriteTxReg(uint8_t);
void UART_DVRT_RX_CallBack(void);
void UART_DVRT_TX_CallBack(void);

// UartX specific defines dependent on MCC and UART used
// Also make sure any UART transmit buffers have sufficient size
#define UART_DVRT_txReg					TX1REG
#define UART_DVRT_rxReg					RC1REG
#define UART_DVRT_FERR					RC1STAbits.FERR
#define UART_DVRT_OERR					RC1STAbits.OERR
#define UART_DVRT_EN()					RC1STAbits.SPEN=1
#define UART_DVRT_DIS()					RC1STAbits.SPEN=0
#define UART_DVRT_RX_ISR_REGISTER		EUSART1_SetRxInterruptHandler
#define UART_DVRT_TX_ISR_REGISTER		EUSART1_SetTxInterruptHandler
#define UART_DVRT_INT_TX_DIS()			PIE3bits.TX1IE = 0
#define UART_DVRT_INT_TX_EN()			PIE3bits.TX1IE = 1
#define UART_DVRT_IsTxDone				EUSART1_is_tx_done

#endif	/* DVRUNTIME_H */

