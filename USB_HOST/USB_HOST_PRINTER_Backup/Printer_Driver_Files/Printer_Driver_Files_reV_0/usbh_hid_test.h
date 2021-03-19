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





#endif /* ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_HID_TEST_H_ */
