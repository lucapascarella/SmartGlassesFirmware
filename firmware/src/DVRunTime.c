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

//#include "mcc_generated_files/mcc.h"

#include "DVRunTime.h"

//------------------------------------------------------------------------------
// VARIABLES

DVPMT_t DVPMT[DVPMT_NUM];		// Dynamic Variable Pointer Monitor Array

volatile union DVCmds {
    DVPMT_Update_t stream;
    DVVarUpdate_t Var;
    DVStreamTickUpdate_t interval;
    DVgenericCmd_t generic;
    uint8_t DVCmdArray[sizeof (DVPMT_Update_t)];
} DVCmdRx;								// DV RX command

volatile uint8_t rxBufPtr, tickCounter;

uint16_t DVStreamInterval, cntTrigTx;
uint16_t DVTimeOutRx, cntTimeOutRx;

struct flagS {
    unsigned streamOn   : 1;	// Streaming On
    unsigned osr        : 1;	// One shot read pending
    unsigned ping       : 1;    // Ping pending
} DVflag;

#if DV_INTERRUPT_DRIVEN_TX==1
EXTERN DV_OutBuff_t DOutBuff;
#endif


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void UART_DVRT_WriteTxReg(uint8_t txData){
	while (UART_DVRT_IsTxDone() != 1){
	}; 
    UART_DVRT_txReg = txData;	
}


// <editor-fold defaultstate="collapsed" desc="DVRT Initialize Function">
void DVRT_Initialize(void) {
uint8_t i;

    DVStreamInterval = DV_STREAM_TIME;
    for (i = 0; i < DVPMT_NUM; i++) {
        DVPMT[i].address = 0;
        DVPMT[i].size = 0;
    }
#if DV_INTERRUPT_DRIVEN_TX==1
    DOutBuff.Buffer[0] = DV_START_FRAME;
    DOutBuff.Buffer[1] = DV_END_FRAME;
    DOutBuff.Buffer[DV_DOB_SIZE - 1] = DV_END_FRAME;
#endif
	
	DVTimeOutRx = DV_RX_CMD_TIMEOUT;
	
	UART_DVRT_RX_ISR_REGISTER(&UART_DVRT_RX_CallBack);
#if DV_INTERRUPT_DRIVEN_TX==1
	UART_DVRT_TX_ISR_REGISTER(&UART_DVRT_TX_CallBack);
#endif
	
}
// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="UART DVRT RX CallBack">
void UART_DVRT_RX_CallBack(void)
{
	if((UART_DVRT_FERR) || (UART_DVRT_OERR)){
		UART_DVRT_DIS();
		UART_DVRT_EN();
		rxBufPtr = 0;
	}
	else{
		DVCmdRx.DVCmdArray[rxBufPtr++] = UART_DVRT_rxReg;
	}	
	
	cntTimeOutRx = 0;
}
// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="UART DVRT TX CallBack">
void UART_DVRT_TX_CallBack(void)
{
#if DV_INTERRUPT_DRIVEN_TX==1
	if(DOutBuff.TxIndex <= DOutBuff.Index){
		
		UART_DVRT_WriteTxReg(DOutBuff.Buffer[DOutBuff.TxIndex]);
		DOutBuff.TxIndex++;
	}
	else{						// completed buffer transmission
		DOutBuff.Index = 0;
		UART_DVRT_INT_TX_DIS();
	}
#endif	
}
// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="DVRT PROCESS Function">
void DVRT_Process(){
	uint8_t i, k;

	if(cntTimeOutRx++ >= DVTimeOutRx){
		cntTimeOutRx = 0;
		DVRT_RxCommand();							// Check RX packet
	}	
	
	if(cntTrigTx++ >= DVStreamInterval){
		cntTrigTx = 0;
	
		if(DVflag.streamOn){				// TX packet
			//IO_Led0_Toggle();

			#if DV_INTERRUPT_DRIVEN_TX==1
			// <editor-fold defaultstate="collapsed" desc="INTERRUPT DRIVEN TX">

			if(DOutBuff.Index == 0){		// is Tx buffer empty ? (previous package transmission completed)
	//			DVRT_SendData();
				DOutBuff.Length = 0;
				DOutBuff.Buffer[DOutBuff.Index++] = DV_START_FRAME;
				DOutBuff.Buffer[DOutBuff.Index++] = tickCounter++;
				if(DVflag.ping){
					DOutBuff.Buffer[DOutBuff.Index++] = ((uint8_t)(DV_ID_FW));
					DOutBuff.Buffer[DOutBuff.Index++] = ((uint8_t)(DV_ID_FW>>8));
				}
				else{
					for (i = 0; i < DVPMT_NUM; i++) {
						for (k = 0; k < DVPMT[i].size; k++) {
							DOutBuff.Buffer[DOutBuff.Index++] = *(DVPMT[i].address + k);
						}		
					}		
				}
				DOutBuff.Buffer[DOutBuff.Index] = DV_END_FRAME;
				DOutBuff.TxIndex = 0;
				UART_DVRT_INT_TX_EN();					// all buffer filled -> start Txmit
			}
		// </editor-fold>		
			#else
			// <editor-fold defaultstate="collapsed" desc="NON INTERRUPT DRIVEN ">

			UART_DVRT_WriteTxReg(DV_START_FRAME);
			UART_DVRT_WriteTxReg(tickCounter++);
			if(DVflag.ping){
				UART_DVRT_WriteTxReg((uint8_t)(DV_ID_FW));
				UART_DVRT_WriteTxReg((uint8_t)(DV_ID_FW>>8));
			}
			else{
				for (i = 0; i < DVPMT_NUM; i++) {
					for (k = 0; k < DVPMT[i].size; k++) {
						UART_DVRT_WriteTxReg(*(DVPMT[i].address + k));
					}		
				}		
			}		
			UART_DVRT_WriteTxReg(DV_END_FRAME);
		// </editor-fold>		
			#endif
		}
	}

	if(DVflag.osr || DVflag.ping){				// One shot reading or ping command execution
		DVflag.osr = 0;
		DVflag.ping = 0;
		DVflag.streamOn = 0;					// stop streaming
	}	
	
}
// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="DVRT RX Command Function">
void DVRT_RxCommand(void) {
    uint8_t VARcount;
	
	if(rxBufPtr >= DV_RX_CMD_MIN_SIZE){
		if((DVCmdRx.DVCmdArray[0] == DV_START_FRAME) && (DVCmdRx.DVCmdArray[rxBufPtr-1] == DV_END_FRAME)){
			switch (DVCmdRx.stream.command) {
				case UDVPMT:			// Update Dynamic Variable Pointer Memory Table
				{
					VARcount = 0;
					while (VARcount < DVCmdRx.stream.size) {
						DVPMT[VARcount].size = DVCmdRx.stream.DVPMT[VARcount].size;
						DVPMT[VARcount].address = DVCmdRx.stream.DVPMT[VARcount].address;
						VARcount++;
					}
					break;
				}
				case UVVAL:				// Update Variable Value
				{
					VARcount = 0;
					while (VARcount < DVCmdRx.Var.size) {
						*(uint8_t *)(DVCmdRx.Var.address + VARcount) = DVCmdRx.Var.dataBuff[VARcount];
						VARcount++;
					}
					break;				
				}	
				case USTICK:
				{
						DVStreamInterval = DVCmdRx.interval.period;
						cntTrigTx = 0;
						break;
				}
				case SOFF:
				{
						DVflag.streamOn = 0;
						break;
				}
				case SON:
				{
						DVflag.streamOn = 1;
						cntTrigTx = DVStreamInterval;	// to replay immediately						
						break;
				}
				case OSR:				// On Shot Reading
				{
						DVflag.osr = 1;
						DVflag.streamOn = 1;
						cntTrigTx = DVStreamInterval;	// to replay immediately						
						break;
				}
				case PING:
				{
						DVflag.ping = 1;
						DVflag.streamOn = 1;
						cntTrigTx = DVStreamInterval;	// to replay immediately
						break;
				}
				
			}
		} 
	}
	rxBufPtr = 0;
}
// </editor-fold>


// EOF