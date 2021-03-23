/*
 * usbh_hid_test.c
 *
 *  Created on: May 21, 2020
 *      Author: VIVEK
 */

#include "usbh_hid_test.h"

#define GET_DEVICE_ID	0
#define GET_PORT_STATUS 1
#define SOFT_RESET 2


#define PRT_INIT 0
#define PRT_SYNC 1
#define PRT_GET_DATA 2
#define PRT_POLL 3
#define SIZE_BUFF 10000
#define VIDPID	1
#define BAUDRATE 2
#define CHN_BAUDRATE 3

extern void MX_USART2_UART_Init(uint32_t Budrate);

static USBH_StatusTypeDef USBH_PRT_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_PRT_SOFProcess(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_PRT_Get_Device_Id(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen);
static USBH_StatusTypeDef USBH_PRT_Get_Port_Status(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen);
static USBH_StatusTypeDef USBH_PRT_Soft_Reset(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen);

static void reverse(uint8_t * temp);
static int CheckCMD(const uint8_t *cmd);
static void I2A(uint8_t *temp, uint32_t num);
static uint32_t A2I(uint8_t *temp);
static uint32_t  checkBudrate(uint8_t *temp);
static uint8_t ChangeBaudrate( uint32_t Budrate );
static int CommandOperation( PRT_HandleTypeDef *PRT_Handle , uint8_t *buff);




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



#define PRT_BOOT_CODE 1


static USBH_StatusTypeDef USBH_PRT_InterfaceInit(USBH_HandleTypeDef *phost){

	// USBH_DeviceTypeDef  device;
	PRT_HandleTypeDef *PRT_Handle;
	phost->pActiveClass->pData = (PRT_HandleTypeDef *)USBH_malloc(sizeof(PRT_HandleTypeDef));
	PRT_Handle = (PRT_HandleTypeDef *) phost->pActiveClass->pData;

	memset(PRT_Handle, 0, sizeof(PRT_HandleTypeDef));

	PRT_Handle->num = 0;
	PRT_Handle->Budrate = Bud_9600;
	PRT_Handle->poll = 10;
	PRT_Handle->vID = phost->device.DevDesc.idVendor;
	PRT_Handle->pID = phost->device.DevDesc.idProduct;
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

	 switch(PRT_Handle->state)
	 {

			case PRT_INIT:
				PRT_Handle->state = PRT_SYNC;
				break;

			 case PRT_SYNC:
				  /* Sync with start of Even Frame */
				if (phost->Timer & 1U)
				 {
					PRT_Handle->state = PRT_GET_DATA;
				 }
				 PRT_Handle->state = PRT_GET_DATA;
				 break;

			 case PRT_GET_DATA:

				 do
				 {
					 memset(buff,0,SIZE_BUFF);
					 HAL_UART_Receive(&huart2, buff, SIZE_BUFF,2000);

				 }while(CommandOperation(PRT_Handle,buff));

				 XferSize = strlen(buff);
				 pktcnt=0;

				 for(int i=0; (pktcnt <= (XferSize/PRT_Handle->length)); i += PRT_Handle->length)
				{
				// Number of packets are important because the max packet size is decided and can not send more than that.
				// Here in this case is 64 bytes. It is already decided by device/configuration structures.
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

				 if(USBH_LL_GetURBState(phost, PRT_Handle->OutPipe) == USBH_URB_DONE)
				 {

					 status = USBH_LL_GetURBState(phost, PRT_Handle->OutPipe);
					 XferSize = USBH_LL_GetLastXferSize(phost, PRT_Handle->OutPipe);
					 PRT_Handle->state = PRT_GET_DATA;
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




static int CheckCMD(const uint8_t *cmd){

	if( !strcmp(cmd,"VIDPID") )
	{
		return 1;
	}else if(!strcmp(cmd,"BAUDRATE") )
	{
		return 2;
	}else if(!strcmp(cmd,"CHN_BAUDRATE"))
	{
		return 3;
	}

	return 0;
}

static void reverse(uint8_t * temp){

	uint8_t len = strlen(temp)-1;
	uint8_t i=0;
	uint8_t j=0;
	while(i < len){
		j = temp[i];
		temp[i++] = temp[len];
		temp[len--] = j;

		//i++;//
		//len--;//
	}

}

static void I2A(uint8_t *temp, uint32_t num){

	uint8_t i=0;
	while(num){
		temp[i++] = num%10+48;
		num /= 10;
	}

	reverse(temp);
}

static uint32_t A2I(uint8_t *temp)
{

	uint32_t num = 0;

	for(int i=0; i < strlen(temp); i++)
	{
		num = num*10 + (temp[i]-48);

	}

	return num;
}

static uint32_t  checkBudrate(uint8_t *temp)
{
	return A2I(temp);
}



static uint8_t ChangeBaudrate( uint32_t Budrate )
{

	MX_USART2_UART_Init(Budrate);

	return 1;
}



static int CommandOperation( PRT_HandleTypeDef *PRT_Handle , uint8_t *buff)
{
	//flag = 0 ; Command Not Found
	//flag = 1 ; Command  Found

	uint8_t temp[64],pktcnt,flag;
	uint16_t XferSize = strlen(buff);
	 pktcnt=flag=0;
	 memset(temp,0,64);

	 if( (XferSize == 0))
		 return 1;

	 if( (XferSize > 15) )
		 return 0;

	 pktcnt = CheckCMD(buff);

	 switch( pktcnt )
		 {
		 case VIDPID:// Get Vid, Pid od device
			 I2A(temp,PRT_Handle->vID);
			 HAL_UART_Transmit(&huart2, &temp, strlen(temp),1000);
			 I2A(temp,PRT_Handle->pID);
			 HAL_UART_Transmit(&huart2, ",", strlen(","),1000);
			 HAL_UART_Transmit(&huart2, &temp, strlen( temp),1000);
			 memset(temp,0,64);
			 flag = 1; //flag = 1 ; Command Found
			 break;

		 case BAUDRATE:// Get current baudrate default 9600
			 I2A(temp,PRT_Handle->Budrate);
			 HAL_UART_Transmit(&huart2, &temp, strlen(temp),1000);
		 	 flag = 1; //flag = 1 ; Command Found
		 	 break;

		 case CHN_BAUDRATE:// Change baudrate

			 HAL_UART_Transmit(&huart2, "Waiting for modified baudrate\n", strlen( "Waiting for modified baudrate\n"),1000);
			 do
			 {
				 memset(temp,0,64);
				 HAL_UART_Receive(&huart2, &temp, 64,1000);

			 }while(!strlen(temp));

			 PRT_Handle->Budrate = checkBudrate(temp);
			 flag = ChangeBaudrate(  PRT_Handle->Budrate );
			 break;
		 default:
			 HAL_UART_Transmit(&huart2, "This is not know Command\n", strlen(  "This is not know Command\n"),1000);
			 flag = 0; //flag = 0 ; Command Not Found
			 break;

		 }//end-Switch



	 return flag;
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



static USBH_StatusTypeDef USBH_PRT_Soft_Reset(USBH_HandleTypeDef *phost, uint8_t *Buff, uint16_t Devlen){

		phost->Control.setup.b.bmRequestType = 0xA1;
		phost->Control.setup.b.bRequest = SOFT_RESET;
		phost->Control.setup.b.wValue.w = 0x0000;
		phost->Control.setup.b.wIndex.w = 0x0000;
		phost->Control.setup.b.wLength.w = Devlen;

		return USBH_CtlReq(phost, Buff, Devlen);
}

