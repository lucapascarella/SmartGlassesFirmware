/******************************************************************************
  IS67 Driver Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_is67.c

  Summary:
    IS67 Driver Interface Definition

  Description:
    The IS67 Driver provides a interface to access the IS67 peripheral on the PIC32
    microcontroller. This file should be included in the project if IS67 driver
    functionality is needed.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "driver/is67/src/drv_is67_local.h"
#include "sys/kmem.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global objects
// *****************************************************************************
// *****************************************************************************

#define SQI_CHIP_SELECT         SQI_BDCTRL_SQICS_CS0

#define DRV_SQI_LANE_MODE       SQI_BDCTRL_MODE_QUAD_LANE

#define CMD_DESC_NUMBER         5
#define DUMMY_BYTE              0xFF

static DRV_IS67_OBJECT gDrvIS67Obj;
static DRV_IS67_OBJECT *dObj = &gDrvIS67Obj;

/* Table mapping the Flash ID's to their sizes. */
static uint32_t gSstMemoryIdSizeTable [3][2] = {
    {0x00, 0x100000}, /*  8 MBit */
    {0x01, 0x200000}, /* 16 MBit */
    {0x02, 0x400000}, /* 32 MBit */
};

static sqi_dma_desc_t CACHE_ALIGN sqiCmdDesc[CMD_DESC_NUMBER];
static sqi_dma_desc_t CACHE_ALIGN sqiBufDesc[DRV_IS67_BUFF_DESC_NUMBER];

//static uint8_t CACHE_ALIGN statusRegVal;

static uint8_t CACHE_ALIGN jedecID[8];

static uint8_t CACHE_ALIGN sqi_cmd_jedec[4];
static uint8_t CACHE_ALIGN sqi_cmd_eqio;
static uint8_t CACHE_ALIGN sqi_cmd_rsten;
static uint8_t CACHE_ALIGN sqi_cmd_rst;
//static uint8_t CACHE_ALIGN sqi_cmd_rdsr[2];
static uint8_t CACHE_ALIGN sqi_cmd_wr[4];
static uint8_t CACHE_ALIGN sqi_cmd_hsr[4];
static uint8_t CACHE_ALIGN sqi_cmd_dummy[2];


// *****************************************************************************
// *****************************************************************************
// Section: IS67 Driver Local Functions
// *****************************************************************************
// *****************************************************************************

static void DRV_IS67_EventHandler(uintptr_t context)
{
    DRV_IS67_OBJECT *obj = (DRV_IS67_OBJECT *)context;

    obj->isTransferDone = true;
}

/* This function returns the flash size in bytes for the specified deviceId. A
 * zero is returned if the device id is not supported. */
static uint32_t DRV_IS67_GetMemorySize( uint8_t deviceId )
{
    uint8_t i = 0;

    for (i = 0; i < 3; i++)
    {
        if (deviceId == gSstMemoryIdSizeTable[i][0])
        {
            return gSstMemoryIdSizeTable[i][1];
        }
    }

    return 0;
}

static void DRV_IS67_ResetFlash(void)
{
    dObj->isTransferDone = false;

    dObj->curOpType = DRV_IS67_OPERATION_TYPE_CMD;

    sqi_cmd_rsten               = (uint8_t)IS67_CMD_FLASH_RESET_ENABLE;

    sqiCmdDesc[0].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(1) | SQI_BDCTRL_PKTINTEN |
                                    SQI_BDCTRL_LASTPKT | SQI_BDCTRL_LASTBD |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DEASSERT |
                                    SQI_BDCTRL_DESCEN);

    sqiCmdDesc[0].bd_bufaddr    = (uint32_t *)KVA_TO_PA(&sqi_cmd_rsten);
    sqiCmdDesc[0].bd_stat       = 0;
    sqiCmdDesc[0].bd_nxtptr     = NULL;

    dObj->is67Plib->DMATransfer((sqi_dma_desc_t *)KVA_TO_PA(&sqiCmdDesc[0]));

    while(dObj->isTransferDone == false)
    {
        /* Wait for transfer to complete */
    }

    dObj->isTransferDone = false;

    sqi_cmd_rst                 = (uint8_t)IS67_CMD_FLASH_RESET;

    sqiCmdDesc[1].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(1) | SQI_BDCTRL_PKTINTEN |
                                    SQI_BDCTRL_LASTPKT | SQI_BDCTRL_LASTBD |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DEASSERT |
                                    SQI_BDCTRL_DESCEN);

    sqiCmdDesc[1].bd_bufaddr    = (uint32_t *)KVA_TO_PA(&sqi_cmd_rst);
    sqiCmdDesc[1].bd_stat       = 0;
    sqiCmdDesc[1].bd_nxtptr     = NULL;

    dObj->is67Plib->DMATransfer((sqi_dma_desc_t *)KVA_TO_PA(&sqiCmdDesc[1]));

    while(dObj->isTransferDone == false)
    {
        /* Wait for transfer to complete */
    }
}

static void DRV_IS67_EnableQuadIO(void)
{
    dObj->isTransferDone = false;

    sqi_cmd_eqio                = (uint8_t)IS67_CMD_ENABLE_QUAD_IO;

    sqiCmdDesc[0].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(1) | SQI_BDCTRL_PKTINTEN |
                                    SQI_BDCTRL_LASTPKT | SQI_BDCTRL_LASTBD |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DEASSERT |
                                    SQI_BDCTRL_DESCEN);

    sqiCmdDesc[0].bd_bufaddr    = (uint32_t *)KVA_TO_PA(&sqi_cmd_eqio);
    sqiCmdDesc[0].bd_stat       = 0;
    sqiCmdDesc[0].bd_nxtptr     = NULL;

    dObj->curOpType = DRV_IS67_OPERATION_TYPE_CMD;

    dObj->is67Plib->DMATransfer((sqi_dma_desc_t *)KVA_TO_PA(&sqiCmdDesc[0]));

    while(dObj->isTransferDone == false)
    {
        /* Wait for transfer to finish */
    }
}

static bool DRV_IS67_ValidateHandleAndCheckBusy( const DRV_HANDLE handle )
{
    /* Validate the handle.
     * If isTransferDone is False then there is an operation in progress.
     */
    if(handle == DRV_HANDLE_INVALID || dObj->isTransferDone == false)
    {
        return true;
    }

    return false;
}
// *****************************************************************************
// *****************************************************************************
// Section: IS67 Driver Global Functions
// *****************************************************************************
// *****************************************************************************

bool DRV_IS67_ReadJedecId( const DRV_HANDLE handle, void *jedec_id)
{
    if(DRV_IS67_ValidateHandleAndCheckBusy(handle) == true)
    {
        return false;
    }

    dObj->isTransferDone = false;

    sqi_cmd_jedec[0]            = (uint8_t)IS67_CMD_JEDEC_ID_READ;
    sqi_cmd_jedec[1]            = DUMMY_BYTE;
    sqi_cmd_jedec[2]            = DUMMY_BYTE;
    sqi_cmd_jedec[3]            = DUMMY_BYTE;

    sqiCmdDesc[0].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(4) | DRV_SQI_LANE_MODE |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DESCEN);

    sqiCmdDesc[0].bd_bufaddr    = (uint32_t *)KVA_TO_PA(&sqi_cmd_jedec);
    sqiCmdDesc[0].bd_stat       = 0;
    sqiCmdDesc[0].bd_nxtptr     = (sqi_dma_desc_t *)KVA_TO_PA(&sqiBufDesc[0]);

    sqiBufDesc[0].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(8) | SQI_BDCTRL_PKTINTEN |
                                    SQI_BDCTRL_LASTPKT | SQI_BDCTRL_LASTBD |
                                    DRV_SQI_LANE_MODE | SQI_BDCTRL_DIR_READ |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DEASSERT |
                                    SQI_BDCTRL_DESCEN);

    sqiBufDesc[0].bd_bufaddr    = (uint32_t *)KVA_TO_PA((uint8_t*)jedec_id);
    sqiBufDesc[0].bd_stat       = 0;
    sqiBufDesc[0].bd_nxtptr     = NULL;

    dObj->curOpType = DRV_IS67_OPERATION_TYPE_READ;

    // Initialize the root buffer descriptor
    dObj->is67Plib->DMATransfer((sqi_dma_desc_t *)KVA_TO_PA(&sqiCmdDesc[0]));

    while(dObj->isTransferDone == false)
    {
        /* Wait for transfer to complete */
    }

    return true;
}

DRV_IS67_TRANSFER_STATUS DRV_IS67_TransferStatusGet( const DRV_HANDLE handle )
{
    DRV_IS67_TRANSFER_STATUS status = DRV_IS67_TRANSFER_ERROR_UNKNOWN;

    if(handle == DRV_HANDLE_INVALID)
    {
        return status;
    }

    if (dObj->isTransferDone == true)
    {
        status = DRV_IS67_TRANSFER_COMPLETED;
    }
    else
    {
        status = DRV_IS67_TRANSFER_BUSY;
    }

    return status;
}

bool DRV_IS67_Read( const DRV_HANDLE handle, void *rx_data, uint32_t rx_data_length, uint32_t address)
{
    uint32_t pendingBytes   = rx_data_length;
    uint8_t *readBuffer     = (uint8_t *)rx_data;
    uint32_t numBytes       = 0;
    uint32_t i              = 0;

    if(DRV_IS67_ValidateHandleAndCheckBusy(handle) == true)
    {
        return false;
    }

    if (rx_data_length > (DRV_IS67_BURST_SIZE * DRV_IS67_BUFF_DESC_NUMBER))
    {
        return false;
    }

    dObj->isTransferDone = false;

    // Construct parameters to issue read command
    sqi_cmd_hsr[0] = (uint8_t)IS67_CMD_HIGH_SPEED_READ;
    sqi_cmd_hsr[1] = (uint8_t)(0xFFU & (address >> 16));
    sqi_cmd_hsr[2] = (uint8_t)(0xFFU & (address >> 8));
    sqi_cmd_hsr[3] = (uint8_t)(0xFFU & (address >> 0));

    sqiCmdDesc[0].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(4) | DRV_SQI_LANE_MODE |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DESCEN);

    sqiCmdDesc[0].bd_bufaddr    = (uint32_t *)KVA_TO_PA(&sqi_cmd_hsr);
    sqiCmdDesc[0].bd_stat       = 0;
    sqiCmdDesc[0].bd_nxtptr     = (sqi_dma_desc_t *)KVA_TO_PA(&sqiCmdDesc[1]);

    sqi_cmd_dummy[0]            = DUMMY_BYTE;
    sqi_cmd_dummy[1]            = DUMMY_BYTE;

    sqiCmdDesc[1].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(2) | DRV_SQI_LANE_MODE |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DESCEN);

    sqiCmdDesc[1].bd_bufaddr    = (uint32_t *)KVA_TO_PA(&sqi_cmd_dummy);
    sqiCmdDesc[1].bd_stat       = 0;
    sqiCmdDesc[1].bd_nxtptr     = (sqi_dma_desc_t *)KVA_TO_PA(&sqiBufDesc[0]);

    while ((i < DRV_IS67_BUFF_DESC_NUMBER) && (pendingBytes > 0))
    {
        if (pendingBytes > DRV_IS67_BURST_SIZE)
        {
            numBytes = DRV_IS67_BURST_SIZE;
        }
        else
        {
            numBytes = pendingBytes;
        }

        sqiBufDesc[i].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(numBytes) | SQI_BDCTRL_PKTINTEN |
                                        DRV_SQI_LANE_MODE | SQI_BDCTRL_DIR_READ |
                                        SQI_CHIP_SELECT | SQI_BDCTRL_DESCEN);

        sqiBufDesc[i].bd_bufaddr    = (uint32_t *)KVA_TO_PA(readBuffer);
        sqiBufDesc[i].bd_stat       = 0;
        sqiBufDesc[i].bd_nxtptr     = (sqi_dma_desc_t *)KVA_TO_PA(&sqiBufDesc[i+1]);

        pendingBytes    -= numBytes;
        readBuffer      += numBytes;
        i++;
    }

    /* The last descriptor must indicate the end of the descriptor list */
    sqiBufDesc[i-1].bd_ctrl         |= (SQI_BDCTRL_LASTPKT | SQI_BDCTRL_LASTBD |
                                        SQI_BDCTRL_DEASSERT);

    sqiBufDesc[i-1].bd_nxtptr       = NULL;

    dObj->curOpType = DRV_IS67_OPERATION_TYPE_READ;

    // Initialize the root buffer descriptor
    dObj->is67Plib->DMATransfer((sqi_dma_desc_t *)KVA_TO_PA(&sqiCmdDesc[0]));

    return true;
}

bool DRV_IS67_Write( const DRV_HANDLE handle, void *tx_data, uint32_t tx_data_length, uint32_t address)
{
    uint32_t pendingBytes   = tx_data_length;
    uint8_t *writeBuffer     = (uint8_t *)tx_data;
    uint32_t numBytes       = 0;
    uint32_t i              = 0;

    
    if(DRV_IS67_ValidateHandleAndCheckBusy(handle) == true)
    {
        return false;
    }
    
    if (tx_data_length > (DRV_IS67_BURST_SIZE * DRV_IS67_BUFF_DESC_NUMBER))
    {
        return false;
    }

    dObj->isTransferDone = false;

    // Construct parameters to issue page program command
    sqi_cmd_wr[0] = (uint8_t)IS67_CMD_QUAD_IO_WRITE;
    sqi_cmd_wr[1] = (uint8_t)(0xFFU & (address >> 16));
    sqi_cmd_wr[2] = (uint8_t)(0xFFU & (address >> 8));
    sqi_cmd_wr[3] = (uint8_t)(0xFFU & (address >> 0));

    sqiCmdDesc[0].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(4) | DRV_SQI_LANE_MODE |
                                    SQI_CHIP_SELECT | SQI_BDCTRL_DESCEN);

    sqiCmdDesc[0].bd_bufaddr    = (uint32_t *)KVA_TO_PA(&sqi_cmd_wr);
    sqiCmdDesc[0].bd_stat       = 0;
    sqiCmdDesc[0].bd_nxtptr     = (sqi_dma_desc_t *)KVA_TO_PA(&sqiBufDesc[0]);
    
    while ((i < DRV_IS67_BUFF_DESC_NUMBER) && (pendingBytes > 0))
    {
        if (pendingBytes > DRV_IS67_BURST_SIZE)
        {
            numBytes = DRV_IS67_BURST_SIZE;
        }
        else
        {
            numBytes = pendingBytes;
        }

        sqiBufDesc[i].bd_ctrl       = ( SQI_BDCTRL_BUFFLEN_VAL(numBytes) | SQI_BDCTRL_PKTINTEN |
                                        DRV_SQI_LANE_MODE | SQI_BDCTRL_DIR_WRITE |
                                        SQI_CHIP_SELECT | SQI_BDCTRL_DESCEN);

        sqiBufDesc[i].bd_bufaddr    = (uint32_t *)KVA_TO_PA(writeBuffer);
        sqiBufDesc[i].bd_stat       = 0;
        sqiBufDesc[i].bd_nxtptr     = (sqi_dma_desc_t *)KVA_TO_PA(&sqiBufDesc[i+1]);

        pendingBytes    -= numBytes;
        writeBuffer     += numBytes;
        i++;
    }

    /* The last descriptor must indicate the end of the descriptor list */
    sqiBufDesc[i-1].bd_ctrl         |= (SQI_BDCTRL_LASTPKT | SQI_BDCTRL_LASTBD |
                                        SQI_BDCTRL_DEASSERT);

    sqiBufDesc[i-1].bd_nxtptr       = NULL;

    
    dObj->curOpType = DRV_IS67_OPERATION_TYPE_WRITE;

    // Initialize the root buffer descriptor
    dObj->is67Plib->DMATransfer((sqi_dma_desc_t *)KVA_TO_PA(&sqiCmdDesc[0]));

    return true;
}

bool DRV_IS67_GeometryGet( const DRV_HANDLE handle, DRV_IS67_GEOMETRY *geometry )
{
    uint32_t memory_size = 0;
    bool status = true;

    if (DRV_IS67_ReadJedecId(handle, (void *)&jedecID) == false)
    {
        status = false;
    }
    else
    {
        if (jedecID[0] != 0x9D && jedecID[1] != 0x5D) /* 9dh => ISSI ANDD 5dh => PASS */
        {
            status = false;
        }
        else
        {
            memory_size = DRV_IS67_GetMemorySize((jedecID[2] >> 5) & 0x07);

            if (memory_size == 0U)
            {
                status = false;
            }

            if(DRV_IS67_START_ADDRESS >= memory_size)
            {
                status = false;
            }
            else
            {
                memory_size = memory_size - DRV_IS67_START_ADDRESS;

                /* Read block size and number of blocks */
                geometry->read_blockSize    = 1;
                geometry->read_numBlocks    = memory_size;

                /* Write block size and number of blocks */
                geometry->write_blockSize   = DRV_IS67_BURST_SIZE;
                geometry->write_numBlocks   = (memory_size / DRV_IS67_BURST_SIZE);

                geometry->numReadRegions    = 1;
                geometry->numWriteRegions   = 1;

                geometry->blockStartAddress = DRV_IS67_START_ADDRESS;
            }
        }
    }

    return status;
}

DRV_HANDLE DRV_IS67_Open( const SYS_MODULE_INDEX drvIndex)
{
    if ((dObj->status != SYS_STATUS_READY) ||
        (dObj->nClients >= DRV_IS67_CLIENTS_NUMBER))
    {
        return DRV_HANDLE_INVALID;
    }

    /* Reset IS67 Flash device */
    DRV_IS67_ResetFlash();

    /* Put IS67 Flash device on QUAD IO Mode */
    DRV_IS67_EnableQuadIO();

    dObj->nClients++;

    return ((DRV_HANDLE)drvIndex);
}

void DRV_IS67_Close( const DRV_HANDLE handle )
{
    if ( (handle != DRV_HANDLE_INVALID) &&
         (dObj->nClients > 0))
    {
        dObj->nClients--;
    }
}
/* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -
   H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/

SYS_MODULE_OBJ DRV_IS67_Initialize
(
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT *const init
)
{
    DRV_IS67_INIT *is67Init = NULL;

    /* Check if the instance has already been initialized. */
    if (dObj->inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    dObj->status    = SYS_STATUS_UNINITIALIZED;

    /* Indicate that this object is in use */
    dObj->inUse     = true;
    dObj->nClients  = 0;

    /* Assign to the local pointer the init data passed */
    is67Init       = (DRV_IS67_INIT *)init;

    /* Initialize the attached memory device functions */
    dObj->is67Plib = is67Init->is67Plib;

    dObj->is67Plib->RegisterCallback(DRV_IS67_EventHandler, (uintptr_t)dObj);

    dObj->isTransferDone    = true;
    dObj->status            = SYS_STATUS_READY;

    /* Return the driver index */
    return ( (SYS_MODULE_OBJ)drvIndex );
}
/* MISRAC 2012 deviation block end */

SYS_STATUS DRV_IS67_Status( const SYS_MODULE_INDEX drvIndex )
{
    /* Return the driver status */
    return (gDrvIS67Obj.status);
}
