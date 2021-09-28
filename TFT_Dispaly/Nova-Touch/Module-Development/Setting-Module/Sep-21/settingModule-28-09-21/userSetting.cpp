
#include "settings.h"




void mynull(void) {
  Serial.println("I am null call back");
}


void  settings :: loadDisplayContent() {
  Serial.printf("Before insertId = %d\nPageIdx = %d\n",  handlePage.insertId, handlePage.pageIdx );

  switch ( handlePage.pageIdx ) {
    case 0:
      handlePage.maxNodeIdx = 0;
      //find insertId index in userMenu
      for (int8_t i = 0; i < 5; ++i ) {
        if ( ( handlePage.insertId != -1) && ( userMenu[handlePage.insertId].id >= 0 ) && ( userMenu[handlePage.insertId].id < 100 ) ) {
          handlePage.temp[i] = userMenu[handlePage.insertId];
          handlePage.insertId = userMenu[handlePage.insertId].nextId;
          handlePage.maxNodeIdx = i + 1;
        }//end-if
      }//end-for
      break;

    default:
      handlePage.maxNodeIdx = 0;
      for (int8_t i = handlePage.insertId, j = handlePage.insertId; i < handlePage.insertId + 5; ++i ) {
        if ( ( handlePage.insertId != -1) && ( userMenu[handlePage.insertId].id >= 0 ) && ( userMenu[handlePage.insertId].id < 100 ) ) {
          handlePage.temp[i % j] = userMenu[handlePage.insertId];
          handlePage.insertId = userMenu[handlePage.insertId].nextId;
          handlePage.maxNodeIdx = i % j + 1;
        }
      }//end-for
      break;
  }//end-switch

  Serial.printf("After insertId = %d\n",  handlePage.insertId );

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
        Serial.println("dot : " + String(handlePage.dot));

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
          handlePage.arrPageIdx[handlePage.pageIdx++] =  handlePage.dot;
          handlePage.insertId =  handlePage.temp[ handlePage.dot].childNode;
          settingPageImage();// This is the base image of setting
          loadDisplayContent();
          handlePage.dot = 0;
          settingIndexBlankFullCircle();
          settingIndexfillCircle( handlePage.dot );
        }//end-if
        else {
          handlePage.temp[handlePage.dot].userCallBack();
        }

        break;
      case ESC :
       if ( handlePage.pageIdx ) {               
         handlePage.insertId =  handlePage.temp[ handlePage.dot].ParentNode ;
         settingPageImage();// This is the base image of setting
         handlePage.dot = handlePage.arrPageIdx[--handlePage.pageIdx];
         handlePage.insertId -=  handlePage.dot ;
         loadDisplayContent();
         settingIndexBlankFullCircle();
         settingIndexfillCircle( handlePage.dot ); 
       }
       else{
        handlePage.pageIdx = 0;
        Serial.println("Please Stop Here...");
       }
      
      break;
    }//end-switch
  }//end-while
}//end-showTFT


void settings :: addUserMenuNode( ) {
  userSettings node;

  node.id = 0;
  node.preNode = -1;
  node.nextId = 1;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 1;
  node.preNode = 0;
  node.nextId = 2;
  node.subPage = 1;
  node.childNode = 7;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);


  node.id = 2;
  node.preNode = 1;
  node.nextId = 3;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 3;
  node.preNode = 2;
  node.nextId = 4;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 4;
  node.preNode = 3;
  node.nextId = 5;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 5;
  node.preNode = 4;
  node.nextId = 6;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 6;
  node.preNode = 5;
  node.nextId = -1;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 7;
  node.preNode = -1;
  node.nextId = 8;
  node.subPage = 0;
  node.ParentNode = 1;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 8;
  node.preNode = 7;
  node.nextId = 9;
  node.subPage = 1;
  node.ParentNode = 1;
  node.childNode = 10;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 9;
  node.preNode = 8;
  node.nextId = -1;
  node.subPage = 0;
  node.ParentNode = 1;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 10;
    node.preNode = -1;
  node.nextId = 11;
  node.subPage = 0;
  node.ParentNode = 8;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 11;
    node.preNode = 10;
  node.nextId = 12;
  node.subPage = 0;
  node.ParentNode = 8;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 12;
  node.preNode = 11;
  node.nextId = -1;
  node.subPage = 0;
  node.ParentNode = 8;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

}





void  settings :: settingUserPageHandler() {
  // 1. validate User -only admin at this time
  tft.fillScreen(TFT_BLACK);
  Enable_Text_Font()
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  //1-> Display base setting page
  settingPageImage();// This is the base image of setting
  settingIndexBlankFullCircle();
  settingIndexfillCircle( handlePage.dot );
  Enable_Text_Font()

  addUserMenuNode( );

  showOnTFT();

 
}//end-settingUserPageHandler()
