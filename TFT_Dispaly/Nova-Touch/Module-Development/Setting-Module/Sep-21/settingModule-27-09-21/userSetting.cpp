
#include "settings.h"




void mynull(void) {
  Serial.println("I am null call back");
}

void  settings :: showOnTFT(){
  handlePage.end = handlePage.temp.size();
  dot=0;
  while(1){
    switch( ButtonTouch() ){
      case Down 
      dot++;
      if(dot >=5 ) dot %= 5;
      settingIndexBlankFullCircle();
      settingIndexfillCircle( dot );
      break;
      case UP :   break;
      case ENT:   break;
      case ESC :  break;
    }
  }
}


void settings :: addUserMenuNode( ) {
  userSettings node;

  node.id = 1;
  node.nextId =2;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 2;
    node.nextId =3;
  node.subPage = 1;
  node.connectedNode.push_back(100);
  node.connectedNode.push_back(101);
  node.connectedNode.push_back(105);
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);


  node.id = 3;
    node.nextId =4;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 4;
    node.nextId =5;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 5;
    node.nextId =6;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 6;
    node.nextId =7;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 7;
  node.nextId =-1;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 100;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 101;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 102;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 103;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 104;
  node.subPage = 0;
  node.operationName = "vivek" + String(node.id);
  node.userCallBack = mynull;
  userMenu.push_back(node);

  node.id = 105;
  node.subPage = 0;
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
  index.push_back(dot);
  settingIndexfillCircle( index.back() );
  Enable_Text_Font()

  addUserMenuNode( );

  
  //find total node for page 1.
  for(uint8_t i=0;  userMenu[i].id < 100; ++i,handlePage.totalNodeStart=i  );
  Serial.println("total==>> " + String(handlePage.totalNodeStart) );

  loadDisplayContent();
  showOnTFT();

  while (1) {
    switch ( ButtonTouch() ) {
      case Down:
        dot++;
        settingIndexBlankFullCircle();
        settingIndexfillCircle( dot );
        break;

      case UP:
        dot--;
        settingIndexBlankFullCircle();
        settingIndexfillCircle( dot );
        break;
      case ENT:
        if (userMenu[dot].subPage) {

        }
        break;

      case ESC:
        if ( handleESCButton( ) == -1 )
          return;
        break;
    }


    yield();
  }//end-while
}//end-settingUserPageHandler()
