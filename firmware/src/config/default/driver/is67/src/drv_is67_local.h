/*******************************************************************************
  IS67 Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_is67_local.h

  Summary:
    IS67 driver local declarations and definitions

  Description:
    This file contains the IS67 driver's local declarations and definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef DRV_IS67_LOCAL_H
#define DRV_IS67_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "configuration.h"
#include "driver/is67/drv_is67.h"
// *****************************************************************************
// *****************************************************************************
// Section: Local Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* IS67 Command set

  Summary:
    Enumeration listing the IS67VF commands.

  Description:
    This enumeration defines the commands used to interact with the IS67VF
    series of devices.

  Remarks:
    None
*/

typedef enum
{
    /* Reset enable command. */
    IS67_CMD_FLASH_RESET_ENABLE = 0x66, // RESET Enable

    /* Command to reset the flash. */
    IS67_CMD_FLASH_RESET        = 0x99, // RESET

    /* Command to Enable QUAD IO */
    IS67_CMD_ENABLE_QUAD_IO     = 0x35, // Enter QPI mode

    /* Command to Reset QUAD IO */
    IS67_CMD_RESET_QUAD_IO      = 0xFF,

    /* Command to read JEDEC-ID of the flash device. */
    IS67_CMD_JEDEC_ID_READ      = 0x9F,

//    /* QUAD Command to read JEDEC-ID of the flash device. */
//    IS67_CMD_QUAD_JEDEC_ID_READ = 0xAF,

    /* Command to perform High Speed Read */
    IS67_CMD_HIGH_SPEED_READ    = 0x0B, // Fast Read

    /* Write enable command. */
//    IS67_CMD_WRITE_ENABLE       = 0x06,

    /* Page Program command. */
    IS67_CMD_WRITE              = 0x02, // Write

            /* Page Program command. */
    IS67_CMD_QUAD_IO_WRITE      = 0x38, // Quad IO Write

    /* Command to read the Flash status register. */
    IS67_CMD_READ_STATUS_REG    = 0x05,

    /* Command to perform sector erase */
    IS67_CMD_SECTOR_ERASE       = 0x20,

    /* Command to perform Bulk erase */
    IS67_CMD_BULK_ERASE_64K     = 0xD8,

    /* Command to perform Chip erase */
    IS67_CMD_CHIP_ERASE         = 0xC7,

//    /* Command to unlock the flash device. */
//    IS67_CMD_UNPROTECT_GLOBAL   = 0x98

} IS67_CMD;

// *****************************************************************************
/* IS67 Driver operations.

  Summary:
    Enumeration listing the IS67 driver operations.

  Description:
    This enumeration defines the possible IS67 driver operations.

  Remarks:
    None
*/

typedef enum
{
    /* Request is a command operation */
    DRV_IS67_OPERATION_TYPE_CMD = 0,

    /* Request is read operation. */
    DRV_IS67_OPERATION_TYPE_READ,

    /* Request is write operation. */
    DRV_IS67_OPERATION_TYPE_WRITE,

    /* Request is erase operation. */
    DRV_IS67_OPERATION_TYPE_ERASE,

} DRV_IS67_OPERATION_TYPE;

/**************************************
 * IS67 Driver Hardware Instance Object
 **************************************/
typedef struct
{
    /* Flag to indicate in use  */
    bool inUse;

    /* Flag to indicate status of transfer */
    volatile bool isTransferDone;

    /* The status of the driver */
    SYS_STATUS status;

    /* Intent of opening the driver */
    DRV_IO_INTENT ioIntent;

    /* Indicates the number of clients that have opened this driver */
    size_t nClients;

    /* Current Operation */
    DRV_IS67_OPERATION_TYPE curOpType;

    /* PLIB API list that will be used by the driver to access the hardware */
    const DRV_IS67_PLIB_INTERFACE *is67Plib;

} DRV_IS67_OBJECT;





#endif //#ifndef DRV_IS67_LOCAL_H

/*******************************************************************************
 End of File
*/

