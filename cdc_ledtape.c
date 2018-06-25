/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/** INCLUDES *******************************************************/
#include "system.h"

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include "usb.h"

#include "app_led_usb_status.h"
#include "app_device_cdc_basic.h"
#include "usb_config.h"

/** VARIABLES ******************************************************/

static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];

/*********************************************************************
* Function: void APP_DeviceCDCBasicDemoInitialize(void);
*
* Overview: Initializes the demo code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCDCBasicDemoInitialize()
{   
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 115200;
}

/*********************************************************************
* Function: void APP_DeviceCDCBasicDemoTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceCDCBasicDemoInitialize() and APP_DeviceCDCBasicDemoStart() demos
*   respectively.
*
* Input: None
*
* Output: None
*
********************************************************************/
#define HIGH() \
  do { \
    PORTC = 1; \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    PORTC = 0; \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
  } while(0)

#define LOW() \
  do { \
    PORTC = 1; \
    asm("nop"); \
    asm("nop"); \
    PORTC = 0; \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
  } while(0)

const uint32_t led[3][30] = {
    {
        0x010000, 0x020000, 0x030000, 0x040000, 0x050000, 0x060000, 0x070000, 0x080000, 0x090000, 0x0a0000,
        0x0b0000, 0x0c0000, 0x0d0000, 0x0e0000, 0x0f0000, 0x100000, 0x110000, 0x120000, 0x130000, 0x140000,
        0x150000, 0x160000, 0x170000, 0x180000, 0x190000, 0x1a0000, 0x1b0000, 0x1c0000, 0x1d0000, 0x1e0000,
    }, {
        0x000100, 0x000200, 0x000300, 0x000400, 0x000500, 0x000600, 0x000700, 0x000800, 0x000900, 0x000a00,
        0x000b00, 0x000c00, 0x000d00, 0x000e00, 0x000f00, 0x001000, 0x001100, 0x001200, 0x001300, 0x001400,
        0x001500, 0x001600, 0x001700, 0x001800, 0x001900, 0x001a00, 0x001b00, 0x001c00, 0x001d00, 0x001e00,
    }, {
        0x000001, 0x000002, 0x000003, 0x000004, 0x000005, 0x000006, 0x000007, 0x000008, 0x000009, 0x00000a,
        0x00000b, 0x00000c, 0x00000d, 0x00000e, 0x00000f, 0x000010, 0x000011, 0x000012, 0x000013, 0x000014,
        0x000015, 0x000016, 0x000017, 0x000018, 0x000019, 0x00001a, 0x00001b, 0x00001c, 0x00001d, 0x00001e,
    },
};

void turnOnLed(const uint32_t *pLed) {
    for (uint8_t i = 0; i < sizeof(led[0]) / sizeof(led[0][0]); ++i) {
        uint32_t mask = 0x800000;
        uint32_t l = pLed[i];
        
        while (mask != 0) {
            if ((l & mask) != 0) HIGH();
            else LOW();

            mask >>= 1;
        }
    }    
}

void APP_DeviceCDCBasicDemoTasks()
{
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
    {
        return;
    }
        
    /* Check to see if there is a transmission in progress, if there isn't, then
     * we can see about performing an echo response to data received.
     */
    if( USBUSARTIsTxTrfReady() == true) {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        for(i=0; i<numBytesRead; i++) {
            uint8_t c = readBuffer[i];
            if ('0' <= c && c <= '2') {
                turnOnLed(led[c - '0']);
            }
        }
    }

    CDCTxService();
}
