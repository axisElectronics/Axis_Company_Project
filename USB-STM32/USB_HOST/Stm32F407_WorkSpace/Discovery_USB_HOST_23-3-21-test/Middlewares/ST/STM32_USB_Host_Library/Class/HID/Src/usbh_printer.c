/*
 * usbh_printer.c
 *
 *  Created on: Jun 15, 2020
 *      Author: VIVEK
 */

#include "usbh_printer.h"

uint8_t dData[2000];

/* @ checkCMD : Check command
 *
 */


int CheckCMD(const uint8_t *cmd){

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
	if( !strcmp(cmd,"PRINTREPORT"))
	{
		return 4;
	}

	return 0;
}

/* @ reverse : Reverse the String
 * @ temp : string address
 */


void reverse(uint8_t * temp){

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

}//end-reverse

/* @ I2A : Change Integer into ASCii
 * @ temp : Destination buffer of String
 * @ num : Integer
 */

void I2A(uint8_t *temp, uint32_t num){

	uint8_t i=0;
	while(num){
		temp[i++] = num%10+48;
		num /= 10;
	}

	reverse(temp);
}

/* @ A2I    : Change ASCii to Integer
 * @ temp   : String buffer
 * @ return : converted Integer
 */

uint32_t A2I(uint8_t *temp)
{

	uint32_t num = 0;

	for(int i=0; i < strlen(temp); i++)
	{
		num = num*10 + (temp[i]-48);

	}

	return num;
}

/* @ checkBaudrate : check what is the baudrate
 * @ temp : source buffer
 * @return : integer value or Baudrate
 */

uint32_t  checkBudrate(uint8_t *temp)
{
	return A2I(temp);
}

/* @ ChangeBaudrate : It is used to change baudrate at run time.
 * @ Baudrate : This is desired baudrate
 */

static uint8_t ChangeBaudrate( uint32_t Baudrate )
{

	MX_USART2_UART_Init(Baudrate);

	return 1;
}

/* @ CommandOperation : This function check that received buffer is a command or not.
 * @ buff : received buffer
 * @ PRT_Handle : Printer Handler Structure
 * @ return : Use as per requirement
 */

int CommandOperation( PRT_HandleTypeDef *PRT_Handle , uint8_t *buff)
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

		 case PRINTREPORT:
			 	 printReport();
			 	 break;
		 default:
		 {
			 HAL_UART_Transmit(&huart2, "This is not know Command\n", strlen(  "This is not know Command\n"),1000);
			 flag = 0; //flag = 0 ; Command Not Found
		 }
		 break;

		 }//end-Switch



	 return flag;
}


uint8_t printReport(void)
{
	/*
	 * Use it as per requirement and Conditions.
	 */

	return 0;
}

/* @ PRT_Report : it convert received buffer data into postScript
 * @ pData :  Received RAW data from User
 * @ return : converted data address into post scripts
 */

uint8_t *PRT_Report(uint8_t *pData)
{

	memset(dData,0,2000);

	  if(strlen(pData) > 15)
		  {
			  uint16_t startIdx =0;   // starting index of Buffer data
			  uint16_t lastIdx =0;    // Last index of Buffer data
			  uint8_t *startAddr; // starting address of Buffer data
			  uint8_t *lastAddr;  // Last address of Buffer data

			  strcat(dData, "/Courier findfont [ 12 0 0 12 0 0] makefont setfont \r\n");
			  strcat(dData, "/showline { gsave show grestore 0 -12  rmoveto } def \r\n");
			  strcat(dData, " 18   745  moveto \r\n");

			  for(int idx = 0; idx < strlen(pData); )
			  {
				  startIdx  =  idx;
				  startAddr = (pData+startIdx);
				  lastAddr   = (uint8_t *)strstr(pData+startIdx, "\r\n" );

				  lastIdx = lastAddr - startAddr;

				  idx = idx  + lastIdx;

				  fun2(pData, startIdx, lastIdx);

				  idx++;
				  idx++;
			  }//end -for(idx)
		  }//end- if()

	  	  strcat(dData," showpage \r\n");
		  strcat(dData, "18   745  moveto \r\n");
		  strcat(dData, " showpage \r\n");

		  HAL_UART_Transmit(&huart2, dData, strlen(dData), 3000);
return dData;

}//end-PRT_Report()


/* @ SendDataOnPrinter : we send whole data to connected printer in the form of packets.
 * @ PRT_Handle : printer handle structure.
 * @ phost : USB Host handle structure
 * @ buff :  received data
 */


uint8_t SendDataOnPrinter(USBH_HandleTypeDef *phost, PRT_HandleTypeDef *PRT_Handle, uint8_t *buff)
{
	uint8_t temp[64],pktcnt=0,status;
	uint16_t XferSize=0;
	memset(temp,0,64);
	XferSize = strlen(buff);
	 for(int i=0; (pktcnt <= (XferSize/PRT_Handle->length)); i += PRT_Handle->length)
	  {
			// Number of packets are important because the max packet size is decided and can not send more than that.
			// Here in this case is 64 bytes. It is already decided by device/configuration structures.
			 for(int j=0; j < PRT_Handle->length; j++)
			 {
				 temp[j] = buff[i+j];
			 }//for(j)
			 PRT_Handle->buff = temp;
			 status = USBH_BulkSendData(phost, PRT_Handle->buff, (uint16_t)PRT_Handle->length, PRT_Handle->InPipe, 0);
			 memset(temp,0,64);
			 USBH_Delay(10);
			 pktcnt++;
	}

	 return status;
}


/* @ fun2 : convert data into postScipt function. It is a helper function.
 * @ pData : Data received from user
 * @ startIdx : stating index
 * @ lastIdx : last  index
 */


void fun2(uint8_t *pData, uint16_t startIdx, uint16_t lastIdx)
{
	uint16_t idx=0;
	uint8_t temp[200];
	memset(temp,0,200);
	strcat(dData,"(");
	while( idx <  lastIdx )
	{
		temp[idx++] =  pData[startIdx++];
	}
	temp[idx] = '\0';
	strcat(dData,temp);
	strcat(dData, ") showline \r\n");
}
