/*
 * usbh_hid_test.h
 *
 *  Created on: May 21, 2020
 *      Author: VIVEK
 */

#ifndef ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_HID_TEST_H_
#define ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_HID_TEST_H_

#include "usbh_core.h"
#include "usbh_conf.h"
#include "usbh_def.h"
#include "usbh_ioreq.h"
#include "usbh_pipes.h"
#include "usbh_ctlreq.h"


extern USBH_ClassTypeDef  PRT_Class;
extern UART_HandleTypeDef huart2;
#define USBH_PRT_CLASS   &PRT_Class
#define USB_PRT_CLASS    0x07U


typedef enum {

	Bud_9600 = 9600,
	Bud_19200 = 19200,
	Bud_115200 = 115200,

}Budrate_Typedef;


typedef struct _PRT_Process{

	uint8_t state;
	uint8_t interface;
	uint8_t num;
	uint8_t OutPipe;
	uint8_t InPipe;
	uint8_t OutEp;
	uint8_t InEp;
	uint8_t ep_addr;
	uint16_t length;
	uint8_t poll;
	uint32_t timer;
	uint8_t DataReady;
	uint8_t *buff;
	uint16_t vID;
	uint16_t pID;
	uint32_t Budrate;
}PRT_HandleTypeDef;




#endif /* ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_HID_TEST_H_ */
