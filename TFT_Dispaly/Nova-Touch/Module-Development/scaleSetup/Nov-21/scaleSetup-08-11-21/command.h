
#ifndef _command_h
#define _command_h

/*  ==============================================================
                  Commands
    =============================================================

    1 @ CMD_STARTDATA :
    --------------------
        >> get continuous data from Machine.

    2 @ CMD_STOPDATA :
    -------------------
        >>  stop data coming from Weight machine
        >>   stop continuous data from machine SO that we didnot overflow buffer.

    3 @ CMD_GETTARE :
    -----------------
        >> get TARE data from Machine
*/

enum {
    STARTDATA = 0,
    STOPDATA  = 1,
    GETTARE   = 2,
    AUTOTARE  = 3,
    ZERODATA  = 4,
    HIGHDATA  = 5, // high precision weight
    LOWDATA   = 6, // low precision weight
  }CMD;

  
class command{

  private :
  char commandBuff[10][20];
  
  public :

  command(){
    //1. Start continuous data command
      strcpy(commandBuff[STARTDATA], "\2DT#1\3");
    //2. Stop continuous data command
      strcpy(commandBuff[STOPDATA], "\2DT#0\3");
    //3. manual Tare weight command
      strcpy(commandBuff[ZERODATA], "\2Z\3");
    //4. get tare weight command
      strcpy(commandBuff[AUTOTARE], "\2T\3");
    //5. auto Tare weight command
      strcpy(commandBuff[GETTARE],  "\2GT\3");
    //6. enable high precision weight
      strcpy(commandBuff[HIGHDATA],  "\2HWT#1\3");
    //7. disable high precision weight
      strcpy(commandBuff[LOWDATA],  "\2HWT#0\3");
  }
  

  int8_t sendCommand(int cmd){
       /* This is command which is responsibale for starting continuous data from machine */ 
        uint32_t timeout = millis();
        char cmdResponse[3];
        int count = 10;
        memset(cmdResponse, '\0', 3);
        
       while(count){
        uint32_t tempTimeOut = millis();
        if( Serial2.available() > 0 ){
          Serial2.readBytes(cmdResponse, 2);
         // Serial.printf("cmdResponse : %s\n", cmdResponse);
          if( !strcmp(cmdResponse, "OK") ) return 1; // command successfull
        }
        if( millis() - timeout > 1000 ){
        Serial2.write( commandBuff[cmd] );
    //    Serial.printf("send commad --->> %s\n",  commandBuff[cmd]);
        timeout = millis();
        --count;
        }
      }//end-while

      return 0;//command faild to get response
  }
  
};


#endif
