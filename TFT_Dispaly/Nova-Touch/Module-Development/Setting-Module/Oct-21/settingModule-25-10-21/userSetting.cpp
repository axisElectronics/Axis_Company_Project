
#include "settings.h"




int8_t mynull( class settings *settings ) {
  Serial.println("I am null call back");
}

int8_t showRect( class settings *settings ) {
  int pId = settings->handlePage.temp[settings->handlePage.dot].id;
//  Serial.printf("pid ==>> %d\n", pId);
//  Serial.println("en***************");
//  for(int8_t i=0; i < 5; ++i ){
//    Serial.print( settings->_getPrtResponse(i) );
//    Serial.print(", ");
//  }
//  Serial.println();
  
 switch(pId){
  case 13: 
          if( settings->_getPrtResponse(0) == '1' ){
              tft.fillRoundRect( 290, 60, 30, 30, 10, TFT_GREEN );
          }else {
              tft.fillRoundRect( 290, 60, 30, 30, 10, TFT_RED );
          } 
          break;
  case 14: 
       
          if( settings->_getPrtResponse(1) == '1' ){
              tft.fillRoundRect( 300, 110, 30, 30, 10, TFT_GREEN );
          }else {
              tft.fillRoundRect( 300, 110, 30, 30, 10, TFT_RED );
          }
          break;
  default : Serial.printf("pid ==>> %d\n", pId); 
  break;
 }
 return 0;
}



int8_t en_saveResponse( class settings *settings ) {

  // At any given time There will only One Printer type will work.So Disable others.
  memset( &settings->userScaleSetup.prtResponse, '0', sizeof(settings->userScaleSetup.prtResponse));
//  Serial.println("saveRespo ==> " + String(settings->handlePage.dot));
  int saveprt = settings->handlePage.arrPageIdx[ settings->handlePage.pageIdx -1 ];
  if(settings->handlePage.dot == 0 )//Enable option has been choose.
    settings->userScaleSetup.prtResponse[saveprt] = '1'; 
  
  settings-> saveIntoSPIFFS();
  
//  Serial.println("en***************");
//  for(int8_t i=0; i < 5; ++i ){
//    Serial.print( settings->_getPrtResponse(i) );
//    Serial.print(", ");
//  }
//  Serial.println();
  
  return 1;
}



void  settings :: loadDisplayContent() {
 // Serial.printf("Before insertId = %d\nPageIdx = %d\n",  handlePage.insertId, handlePage.pageIdx );

  switch ( handlePage.pageIdx ) {
    case 0:
      handlePage.maxNodeIdx = 0;
      //find insertId index in userMenu
      for (int8_t i = 0; i < 5; ++i ) {
        if ( ( handlePage.insertId != -1) && ( userMenu[handlePage.insertId].id >= 0 ) && ( userMenu[handlePage.insertId].id < 100 ) ) {
          handlePage.temp[i] = userMenu[handlePage.insertId];
          handlePage.insertId = userMenu[handlePage.insertId].nextNode;
          handlePage.maxNodeIdx = i + 1;
        }//end-if
      }//end-for
      break;

    default:
      handlePage.maxNodeIdx = 0;
      for (int8_t j = 0; j < 5; ++j ) {
        if ( ( handlePage.insertId != -1) ){
          handlePage.temp[j]  = userMenu[handlePage.insertId];
          handlePage.insertId   = userMenu[handlePage.insertId].nextNode;  
          handlePage.maxNodeIdx = j+1; 
//          Serial.printf("AddingNode ==> %d\n", handlePage.insertId );      
        }else{
          ListNode node = {0};
          handlePage.temp[j]  = node;
        }
      }//end-for
      break;
  }//end-switch

 // Serial.printf("After insertId = %d\n",  handlePage.insertId );

  for (uint8_t strpos = 0; strpos < handlePage.maxNodeIdx; ++strpos) {
    tft.setCursor( basesetting.xText, ( 85 + 54 * strpos ) );
    tft.print( handlePage.temp[strpos].operationName );
  }

}//end- loadDisplayContent



void  settings :: showOnTFT() {

  handlePage.dot = handlePage.arrPageIdx[handlePage.pageIdx];
  settingIndexBlankFullCircle();
  settingIndexfillCircle( handlePage.dot );

  loadDisplayContent();

  while (1) {
    switch ( ButtonTouch() ) {
      case Down:
        ++handlePage.dot;
        if (  handlePage.dot <  handlePage.maxNodeIdx ) {
          if (handlePage.dot >= 5 ) handlePage.dot %= 5;
          settingIndexBlankFullCircle();
          settingIndexfillCircle( handlePage.dot );
        }
        else {
          if ( handlePage.insertId != -1 ) {
            settingPageImage();// This is the base image of setting
            loadDisplayContent();
            if (handlePage.dot >= 5 ) handlePage.dot %= 5;
            settingIndexBlankFullCircle();
            settingIndexfillCircle( handlePage.dot );
          }
          else {
            --handlePage.dot;
          }
        }
        break;

      case UP :
        handlePage.dot--;
     //   Serial.println("dot : " + String(handlePage.dot));

        if ( handlePage.dot <= 0 ) {
          if (  handlePage.temp[0].preNode != -1 ) {
            handlePage.insertId = handlePage.temp[0].preNode;
            settingPageImage();// This is the base image of setting
            loadDisplayContent();
          }
          handlePage.dot = 0;
        }
        settingIndexBlankFullCircle();
        settingIndexfillCircle( handlePage.dot );
        break;

      case ENT:
        if ( handlePage.temp[handlePage.dot].subPage ) {
          uint8_t showFlag=false;
          handlePage.arrPageIdx[handlePage.pageIdx++] =  handlePage.dot;
          handlePage.insertId =  handlePage.temp[ handlePage.dot].childNode;
          //Dynamically changed parent id for Enabling and Disabling
           _checkChildId    =   handlePage.insertId;     
           if(  _checkChildId == 15 ){
             _callingFunctionId = handlePage.temp[ handlePage.dot].id;
             userMenu[15].ParentNode = _callingFunctionId;
             userMenu[16].ParentNode = _callingFunctionId;
           }//end-if
          settingPageImage();// This is the base image of setting
          loadDisplayContent();  
          handlePage.dot = 0;
          settingIndexBlankFullCircle();
          settingIndexfillCircle( handlePage.dot );
          // show enable and Disable option.
            //1. check load structure into handPage.child has enable/disable id
            for(handlePage.dot=0; handlePage.dot < handlePage.maxNodeIdx; ++handlePage.dot ){ 
              if( handlePage.temp[handlePage.dot].childNode == 15 ){
                handlePage.temp[handlePage.dot].userCallBack(this);
              }//end-if
            }//end-for
            handlePage.dot=0;
        }//end-if
        else {
          handlePage.temp[handlePage.dot].userCallBack(this);
            int dot = handlePage.dot;                    
            handlePage.insertId =  handlePage.temp[ handlePage.dot ].id;         
            while( dot ){
                handlePage.insertId =  userMenu[  handlePage.insertId ].preNode;
                --dot;
            }//end-while            
      
          loadDisplayContent();
          settingIndexBlankFullCircle();
          settingIndexfillCircle( handlePage.dot );          
        }//end-else

        break;
      case ESC :
       if ( handlePage.pageIdx ) {               
         handlePage.insertId =  handlePage.temp[ handlePage.dot].ParentNode ;

         settingPageImage();// This is the base image of setting
         handlePage.dot = handlePage.arrPageIdx[--handlePage.pageIdx];
        
         for(int8_t i=0; i < handlePage.dot; ++i ){         
          handlePage.insertId =   userMenu[handlePage.insertId].preNode;
         }         
          int preInsertId = handlePage.insertId;
          
         loadDisplayContent();
         settingIndexBlankFullCircle();
         settingIndexfillCircle( handlePage.dot ); 
         _saveStatus = handlePage.dot;
        
         for( handlePage.dot=0;  handlePage.dot < handlePage.maxNodeIdx; ++handlePage.dot ){
         _checkChildId = ( userMenu[preInsertId+handlePage.dot].subPage == YES )? userMenu[preInsertId+handlePage.dot].childNode : 0;  
          if(  _checkChildId == 15 ){
            Serial.printf("preInsertId >> %d\n", preInsertId+handlePage.dot);
            handlePage.temp[handlePage.dot].userCallBack(this);  
          }//end-if(checkChildId)
         }//end-for
         handlePage.dot = _saveStatus;
       }
       else{
        handlePage.pageIdx = 0;
        Serial.println("Please Stop Here...");
        
        return;
       }
      
      break;
    }//end-switch
  }//end-while
}//end-showTFT


void  settings :: settingUserPageHandler() {
  //User is Authorised now.
    author = 1; 
  // 1. validate User -only admin at this time
  tft.fillScreen(TFT_BLACK);
  Enable_Text_Font()
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  //1-> Display base setting page
  settingPageImage();// This is the base image of setting
  settingIndexBlankFullCircle();
  settingIndexfillCircle( handlePage.dot );
  Enable_Text_Font()
  

  
                  
    ListNode node[MAX_NODE] = {
     /* id, nextNode, preNode, ParentNode, childNode, operationName         ,  subPage,  callBackFuncation */                    
      {  0,        4,      -1,         -1,         1, "Printer Configration",      YES,  mynull },
      {  1,        2,      -1,          0,         5, "Header"              ,      YES,  mynull },
      {  2,        3,       1,          0,        10, "Footer"              ,      YES,  mynull },
      {  3,       -1,       2,          0,        -1, "Product"             ,       NO,  addProductName },
      {  4,       -1,       0,         -1,        13, "Printer Type"        ,       YES,  mynull },
     /* id, nextNode, preNode, ParentNode, childNode, operationName         ,  subPage,  callBackFuncation */   
      {  5,        6,      -1,          1,        -1, "Header 1"            ,       NO,  addHeaderOne },
      {  6,        7,       5,          1,        -1, "Header 2"            ,       NO,  addHeaderTwo },
      {  7,        8,       6,          1,        -1, "Header 3"            ,       NO,  addHeaderThree },
      {  8,        9,       7,          1,        -1, "Header 4"            ,       NO,  addHeaderFour },
      {  9,       -1,       8,          1,        -1, "Header 5"            ,       NO,  addHeaderFive },
      /* id, nextNode, preNode, ParentNode, childNode, operationName         ,  subPage,  callBackFuncation */   
      {  10,       11,      -1,          2,        -1, "Footer 1"            ,      NO,  addFooterOne },
      {  11,       12,      10,          2,        -1, "Footer 2"            ,      NO,  addFooterTwo },
      {  12,       -1,      11,          2,        -1, "Footer 3"            ,      NO,  addFooterThree },
      /* id, nextNode, preNode, ParentNode, childNode, operationName         ,  subPage,  callBackFuncation */   
      {  13,       14,      -1,          4,        15, "RP-45"               ,     YES,  showRect },
      {  14,       -1,      13,          4,        15, "Gainscha"            ,     YES,  showRect },
      /* id, nextNode, preNode, ParentNode, childNode, operationName         ,  subPage,  callBackFuncation */                               
      {  15,       16,      -1,          -2,       -1, "Enable"              ,     NO,  en_saveResponse },
      {  16,       -1,      15,          -2,       -1, "Disable"             ,     NO,  en_saveResponse },
    };

 
  for(int i=0; i < MAX_NODE; ++i ){ 
     userMenu[i] = node[i];
  }

 
  showOnTFT();

 // userMenu.clear();
  memset(&handlePage.temp, 0, sizeof(handlePage.temp)); 
  memset(&userMenu, 0, sizeof(userMenu));
  memset(&tempuserMenu, 0, sizeof(tempuserMenu));
  memset(&node, 0, sizeof(node)); 
  memset(&handlePage, 0, sizeof(handlePage)); 
  
}//end-settingUserPageHandler()
