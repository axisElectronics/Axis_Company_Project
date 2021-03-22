/*
 * usbh_printer.h
 *
 *  Created on: Jun 15, 2020
 *      Author: VIVEK
 */

#ifndef ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_PRINTER_H_
#define ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_PRINTER_H_


#include "stdio.h"
#include "stdint.h"
#include "usbh_hid_test.h"
#include "string.h"


#define GET_DEVICE_ID	0
#define GET_PORT_STATUS 1
#define SOFT_RESET 2
#define PRT_BOOT_CODE 1

#define PRT_INIT 0
#define PRT_SYNC 1
#define PRT_GET_DATA 2
#define PRT_POLL 3
#define SIZE_BUFF 10000
#define VIDPID	1
#define BAUDRATE 2
#define CHN_BAUDRATE 3
#define PRINTREPORT 4

static void reverse(uint8_t * temp);
static int CheckCMD(const uint8_t *cmd);
static void I2A(uint8_t *temp, uint32_t num);
static uint32_t A2I(uint8_t *temp);
static uint32_t  checkBudrate(uint8_t *temp);
static uint8_t ChangeBaudrate( uint32_t Budrate );

int CommandOperation( PRT_HandleTypeDef *PRT_Handle , uint8_t *buff);
uint8_t *PRT_Report(uint8_t *pData);
uint8_t SendDataOnPrinter(USBH_HandleTypeDef *phost, PRT_HandleTypeDef *PRT_Handle, uint8_t *buff);
void fun2(uint8_t *pData, uint16_t startIdx, uint16_t lastIdx);
uint8_t  printReport(void);


#endif /* ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_PRINTER_H_ */
