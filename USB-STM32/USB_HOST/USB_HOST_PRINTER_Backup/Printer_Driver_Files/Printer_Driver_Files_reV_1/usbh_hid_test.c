/*
 * usbh_hid_test.c
 *
 *  Created on: May 21, 2020
 *      Author: VIVEK
 */

#include "usbh_hid_test.h"

#define GET_DEVICE_ID	 0
#define GET_PORT_STATUS	 1
#define SOFT_RESET		 2
#define PRT_INIT		 0
#define PRT_SYNC		 1
#define PRT_GET_DATA	 2
#define PRT_POLL		 3
#define SIZE_BUFF		 10000

static USBH_StatusTypeDef USBH_PRT_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_SOFProcess(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_PRT_Get_Device_Id(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen);
static USBH_StatusTypeDef USBH_PRT_Get_Port_Status(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen);
static USBH_StatusTypeDef USBH_PRT_Soft_Reset(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen);


USBH_ClassTypeDef  PRT_Class =
{
  "PRT",
  USB_PRT_CLASS,
  USBH_PRT_InterfaceInit,
  USBH_PRT_InterfaceDeInit,
  USBH_PRT_ClassRequest,
  USBH_PRT_Process,
  USBH_PRT_SOFProcess,
  NULL,
};

//
//typedef struct _PRT_Process{
//	uint8_t state;
//	uint8_t interface;
//	uint8_t num;
//	uint8_t OutPipe;
//	uint8_t InPipe;
//	uint8_t OutEp;
//	uint8_t InEp;
//	uint8_t ep_addr;
//	uint16_t length;
//	uint8_t poll;
//	uint32_t timer;
//	uint8_t DataReady;
//	uint8_t *buff;
//}PRT_HandleTypeDef;

#define PRT_BOOT_CODE 1


static USBH_StatusTypeDef USBH_PRT_InterfaceInit(USBH_HandleTypeDef *phost){

	// USBH_DeviceTypeDef  device;
	PRT_HandleTypeDef *PRT_Handle;
	phost->pActiveClass->pData = (PRT_HandleTypeDef *)USBH_malloc(sizeof(PRT_HandleTypeDef));
	PRT_Handle = (PRT_HandleTypeDef *) phost->pActiveClass->pData;

	memset(PRT_Handle, 0, sizeof(PRT_HandleTypeDef));

	PRT_Handle->num = 0;
	 PRT_Handle->poll = 10;
	PRT_Handle->interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, PRT_BOOT_CODE, 0xFFU);
	PRT_Handle->ep_addr   = phost->device.CfgDesc.Itf_Desc[PRT_Handle->interface].Ep_Desc[0].bEndpointAddress;
    PRT_Handle->length    = phost->device.CfgDesc.Itf_Desc[PRT_Handle->interface].Ep_Desc[0].wMaxPacketSize;

    PRT_Handle->InEp = (phost->device.CfgDesc.Itf_Desc[PRT_Handle->interface].Ep_Desc[0].bEndpointAddress);
    PRT_Handle->InPipe = USBH_AllocPipe(phost, PRT_Handle->InEp);

    PRT_Handle->OutEp = (phost->device.CfgDesc.Itf_Desc[PRT_Handle->interface].Ep_Desc[1].bEndpointAddress);
    PRT_Handle->OutPipe  = USBH_AllocPipe(phost, PRT_Handle->OutEp);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
   if(phost->pActiveClass->ClassCode == USB_PRT_CLASS){
	   HAL_UART_Transmit(&huart2, (uint8_t *)"\nPrinter class\r\n",(uint16_t)strlen("\nPrinter class\r\n"),( uint32_t) 500);
   }else{
	   HAL_UART_Transmit(&huart2, (uint8_t *)"\nNOT Printer class\r\n",(uint16_t)strlen("\nNOT Printer class\r\n"),( uint32_t) 500);
   }


	//	HAL_UART_Transmit(&huart2, (uint8_t *)phost->device.CfgDesc_Raw,(uint16_t)255 ,( uint32_t)1000);

   /* Decode endpoint IN and OUT address from interface descriptor */
     for (int num = 0; num < 2; num++)
     {
	   if (phost->device.CfgDesc.Itf_Desc[PRT_Handle->interface].Ep_Desc[num].bEndpointAddress & 0x80U)
		  {
			/* Open pipe for IN endpoint */
			USBH_OpenPipe(phost, PRT_Handle->InPipe, PRT_Handle->InEp, phost->device.address,
						  phost->device.speed, USB_EP_TYPE_BULK, PRT_Handle->length);

			USBH_LL_SetToggle(phost, PRT_Handle->InPipe, 0U);
		  } else {



			   /* Open pipe for OUT endpoint */
			   USBH_OpenPipe(phost, PRT_Handle->OutPipe, PRT_Handle->OutEp, phost->device.address, phost->device.speed, USB_EP_TYPE_BULK,PRT_Handle->length);


			   USBH_LL_SetToggle(phost, PRT_Handle->OutPipe, 0U);

		  }
     }

return USBH_OK;
}


static USBH_StatusTypeDef USBH_PRT_InterfaceDeInit(USBH_HandleTypeDef *phost){

	 PRT_HandleTypeDef *PRT_Handle = (PRT_HandleTypeDef *) phost->pActiveClass->pData;

	  if (PRT_Handle->InPipe != 0x00U)
	  {
	    USBH_ClosePipe(phost, PRT_Handle->InPipe);
	    USBH_FreePipe(phost, PRT_Handle->InPipe);
	    PRT_Handle->InPipe = 0U;     /* Reset the pipe as Free */
	  }

	  if (PRT_Handle->OutPipe != 0x00U)
	  {
	    USBH_ClosePipe(phost, PRT_Handle->OutPipe);
	    USBH_FreePipe(phost, PRT_Handle->OutPipe);
	    PRT_Handle->OutPipe = 0U;     /* Reset the pipe as Free */
	  }

	  if (phost->pActiveClass->pData)
	  {
	    USBH_free(phost->pActiveClass->pData);
	    phost->pActiveClass->pData = 0U;
	  }





return USBH_OK;
}


static USBH_StatusTypeDef USBH_PRT_ClassRequest(USBH_HandleTypeDef *phost){

	uint8_t buff[150];
	memset(buff,0,150);

// some printers required these cammands and some are not. these commands give us some usefull infomation only.
// Although that infomation we don't required any where in this Driver.

	while( USBH_PRT_Get_Device_Id(phost, buff, 150) != USBH_OK);
	HAL_UART_Transmit(&huart2, (uint8_t *)buff,(uint16_t)100 ,( uint32_t)500);
	memset(buff,0,150);

	while( USBH_PRT_Get_Port_Status(phost, buff, 150) != USBH_OK);
	HAL_UART_Transmit(&huart2, (uint8_t *)buff,(uint16_t)100 ,( uint32_t)500);
	memset(buff,0,150);

//	while( USBH_PRT_Soft_Reset(phost, buff, 1) != USBH_OK);
//	HAL_UART_Transmit(&huart2, (uint8_t *)buff,(uint16_t)100 ,( uint32_t)500);
//	memset(buff,0,150);

	return USBH_OK;
}


static USBH_StatusTypeDef USBH_PRT_Process(USBH_HandleTypeDef *phost){

	 uint16_t XferSize=0;
	 uint8_t temp[64],pktcnt;
	 uint8_t buff[SIZE_BUFF];

	 USBH_StatusTypeDef status = USBH_OK;
	 PRT_HandleTypeDef *PRT_Handle = (PRT_HandleTypeDef *) phost->pActiveClass->pData;

	 memset(temp,0,64);

	 PRT_Handle->poll = 10;

	 switch(PRT_Handle->state){

	case PRT_INIT:
		PRT_Handle->state = PRT_SYNC;
					break;

	 case PRT_SYNC:
	      /* Sync with start of Even Frame */

		 if (phost->Timer & 1U)
	      {
	        PRT_Handle->state = PRT_GET_DATA;
	      }

		 memset(buff,0,SIZE_BUFF);
		 while(strlen(buff) < 10)
			 HAL_UART_Receive(&huart2, buff, SIZE_BUFF,1000);

		 PRT_Handle->state = PRT_GET_DATA;
	      break;

	 case PRT_GET_DATA:
		 pktcnt=0;
		 XferSize = strlen(buff)+1;
		 // Number of packets are important because the max packet size is decided and can not send more than that.
		 // Here in this case is 64 bytes. It is already decided by device/configuration structures.
	for(int i=0; (pktcnt <= (XferSize/PRT_Handle->length)); i += PRT_Handle->length)
	{
		 for(int j=0; j < PRT_Handle->length; j++)
		 {
				 temp[j] = buff[i+j];
		 }//for(j)

		 PRT_Handle->buff = temp;
		 status = USBH_BulkSendData(phost, PRT_Handle->buff, (uint16_t)PRT_Handle->length, PRT_Handle->OutPipe, 0);
		 memset(temp,0,64);
		 USBH_Delay(10);
		 pktcnt++;

	}

	 PRT_Handle->state = PRT_POLL;
	 PRT_Handle->timer = phost->Timer;
	 PRT_Handle->DataReady = 0U;
	 break;

	 case PRT_POLL:

		 if(USBH_LL_GetURBState(phost, PRT_Handle->OutPipe) == USBH_URB_DONE){

			 status = USBH_LL_GetURBState(phost, PRT_Handle->OutPipe);
			 XferSize = USBH_LL_GetLastXferSize(phost, PRT_Handle->OutPipe);
			 PRT_Handle->state = PRT_INIT;
			 USBH_Delay( PRT_Handle->poll);
		 }

		 break;

	 default:
		break;

	}



	return USBH_OK;
}
static USBH_StatusTypeDef USBH_PRT_SOFProcess(USBH_HandleTypeDef *phost){

	 PRT_HandleTypeDef *PRT_Handle = (PRT_HandleTypeDef *) phost->pActiveClass->pData;

	  if (PRT_Handle->state == PRT_POLL)
	  {
	    if ((phost->Timer - PRT_Handle->timer) >= PRT_Handle->poll)
	    {
	      PRT_Handle->state = PRT_INIT;

	    }
	  }



	return USBH_OK;
}



static USBH_StatusTypeDef USBH_PRT_Get_Device_Id(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen){

		phost->Control.setup.b.bmRequestType = 0xA1;
		phost->Control.setup.b.bRequest = GET_DEVICE_ID;
		phost->Control.setup.b.wValue.w = 0x0000;
		phost->Control.setup.b.wIndex.w = 0x0000;
		phost->Control.setup.b.wLength.w = Devlen;

		return USBH_CtlReq(phost, Buff, Devlen);
}


static USBH_StatusTypeDef USBH_PRT_Get_Port_Status(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen){

		phost->Control.setup.b.bmRequestType = 0xA1;
		phost->Control.setup.b.bRequest = GET_PORT_STATUS;
		phost->Control.setup.b.wValue.w = 0x0000;
		phost->Control.setup.b.wIndex.w = 0x0000;
		phost->Control.setup.b.wLength.w = Devlen;

		return USBH_CtlReq(phost, Buff, Devlen);
}


#define SOFT_RESET 2
static USBH_StatusTypeDef USBH_PRT_Soft_Reset(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen){

		phost->Control.setup.b.bmRequestType = 0xA1;
		phost->Control.setup.b.bRequest = SOFT_RESET;
		phost->Control.setup.b.wValue.w = 0x0000;
		phost->Control.setup.b.wIndex.w = 0x0000;
		phost->Control.setup.b.wLength.w = Devlen;

		return USBH_CtlReq(phost, Buff, Devlen);
}

