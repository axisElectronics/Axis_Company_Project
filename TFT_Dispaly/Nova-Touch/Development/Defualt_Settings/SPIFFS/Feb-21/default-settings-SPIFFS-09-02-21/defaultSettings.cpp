
#include "defaultSetting.h"

int8_t defaultSettings:: initDefaultSettings( )
{

  if (!SPIFFS.begin()) {
    Serial.println("Card Mount Failed");
    return -1;
  }

  if ( !SPIFFS.exists("/Configfile.txt") )
  {
    Serial.println("File is missing. So I am creating fresh file.");
    settingFile = SPIFFS.open("/Configfile.txt", FILE_WRITE);
  }
  else
  {
    settingFile = SPIFFS.open("/Configfile.txt");
  }


  if (!settingFile)
  {
    Serial.println("Error while opening file : ");
    return -1;
  }

  updateDefaultSettings( );

  if ( !SPIFFS.exists("/userConfigfile.txt") )
  {
    Serial.println("File is missing. So I am creating fresh file.");
    settingFile = SPIFFS.open("/userConfigfile.txt", FILE_WRITE);
    settingFile.close();
  }
 
  return 1;
}

void defaultSettings:: updateDefaultSettings( )
{
  settingFile = SPIFFS.open("/Configfile.txt");

  if (!settingFile)
  {
    Serial.println("Error while opening file : ");
    return;
  }

  // 1. Application Setup
  ScaleSetup.application[WeighingMode] =  0;
  ScaleSetup.application[CountingMode] =  1;
  ScaleSetup.application[CheckWeighing] = 0;
  ScaleSetup.application[PriceWeighing] = 0;

  // 2. scale setup
  strcpy( ScaleSetup.dateString, "02/10/2021" );
  ScaleSetup.autoZero = 2;
  ScaleSetup.stabilityFactor = 2;

  ScaleSetup.comPara[Baudrate] = 2;
  ScaleSetup.comPara[DataBits] = 0;
  ScaleSetup.comPara[Parity] = 0;
  ScaleSetup.comPara[StopBits] = 0;

  //3. Weighing Units
  ScaleSetup.WeighingUnits[kg] = 1;
  ScaleSetup.WeighingUnits[gm] = 0;
  ScaleSetup.WeighingUnits[tons] = 0;
  ScaleSetup.WeighingUnits[Ounces] = 0;
  ScaleSetup.WeighingUnits[Pounds] = 0;

  //4. scale Information
  strcpy( ScaleSetup.scaleInformation[ developer ], "Vivek Yadav" );
  strcpy( ScaleSetup.scaleInformation[ baseVersion ], "novaTouch0.0.0" );
  strcpy( ScaleSetup.scaleInformation[ modifiedVersion ], "Under Dev" );

  settingFile.write((byte *)&ScaleSetup, sizeof(ScaleSetup));

  settingFile.close();
}


String defaultSettings:: _getApplication()
{
  String temp;
  int8_t idx;
 
  settingFile = SPIFFS.open("/userConfigfile.txt", FILE_READ);
  if (!settingFile)
  {
    Serial.println("\n_getApplication() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  for ( idx = 0; idx < 4; idx++ )
  {
    if (  userScaleSetup.application[idx] ) break;
  }

  switch (idx)
  {
    case 0: temp = "Weighing Mode"; break;
    case 1: temp = "Counting Mode"; break;
    case 2: temp = "Check Weighing"; break;
    case 3: temp = "Price Weighing"; break;
    default :  temp = "None"; break;
  }
  settingFile.close();
  return temp;
}


int8_t defaultSettings:: _setApplication(int8_t application)
{
  if ( application >= 4) return -1;

  settingFile = SPIFFS.open("/userConfigfile.txt", "w+");


  for (int8_t idx = 0; idx < 4; idx++ )
    userScaleSetup.application[ idx ] =  0;

  userScaleSetup.application[ application ] =  1;

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

//  Serial.println( "-->>> " + String( SPIFFS.info() );

  return  userScaleSetup.application[ application ];
}
