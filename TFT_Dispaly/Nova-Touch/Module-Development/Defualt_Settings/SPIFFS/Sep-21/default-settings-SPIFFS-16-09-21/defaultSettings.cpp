
#include "defaultSetting.h"

File settingFile;
char factorySettingsFilePath[] = "/Configfile.txt";
char userSettingFilePath[] =  "/userConfigfile.txt";



int8_t defaultSettings:: initDefaultSettings( )
{

  if ( !SPIFFS.begin(true) ) {
    Serial.println("Card Mount Failed");
    return -1;
  }

  if ( !SPIFFS.exists( factorySettingsFilePath ) )
  {
    Serial.println("File is missing. So I am creating fresh file.");
    settingFile = SPIFFS.open(factorySettingsFilePath, FILE_WRITE);
  }
  else
  {
    settingFile = SPIFFS.open(factorySettingsFilePath);
    if (!settingFile)
    {
      Serial.println("Error while opening file : ");
      return -1;
    }
  }

  updateDefaultSettings( );

  if ( !SPIFFS.exists(userSettingFilePath) )
  {
    Serial.println("File is missing. So I am creating fresh file.");
    settingFile = SPIFFS.open(userSettingFilePath, FILE_WRITE);
    settingFile.close();
  }

  return 1;
}


/***************************************************************


 ****************************************************************/


void defaultSettings:: updateDefaultSettings( )
{

  settingFile = SPIFFS.open(factorySettingsFilePath, "r+");

  if (!settingFile)
  {
    Serial.println("Error while opening file : ");
    return;
  }

  // 1. Application Setup
  ScaleSetup.application[WeighingMode_]  =  1;
  ScaleSetup.application[CountingMode_]  =  1;
  ScaleSetup.application[CheckWeighing_] =  1;
  ScaleSetup.application[PriceWeighing_] =  1;

  // 2. scale setup
  strcpy( ScaleSetup.dateString, "12/03/21" );
  ScaleSetup.autoZero = 2;
  ScaleSetup.stabilityFactor = 2;

  ScaleSetup.comPara[BAUDRATE] = 2;
  ScaleSetup.comPara[DataBits] = 0;
  ScaleSetup.comPara[Parity]   = 0;
  ScaleSetup.comPara[StopBits] = 0;

  //3. Weighing Units
  ScaleSetup.WeighingUnits[kg]     = 1;
  ScaleSetup.WeighingUnits[gm]     = 0;
  ScaleSetup.WeighingUnits[tons]   = 0;
  ScaleSetup.WeighingUnits[Ounces] = 0;
  ScaleSetup.WeighingUnits[Pounds] = 0;


  //4. scale Information
  strcpy( ScaleSetup.scaleInformation[ developer ], "Vivek Yadav" );
  strcpy( ScaleSetup.scaleInformation[ baseVersion ], "novaTouch0.0.0" );
  strcpy( ScaleSetup.scaleInformation[ modifiedVersion ], "Under Dev" );

  //5. Calibration setup
  ScaleSetup.decimal[0] = '3';

  settingFile.write((byte *)&ScaleSetup, sizeof(ScaleSetup));

  settingFile.close();
}

/***************************************************************


 ****************************************************************/

String defaultSettings:: _getApplication()
{
  String temp;
  int8_t idx;
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, FILE_READ );
  if (!settingFile)
  {
    Serial.println("\n_getApplication() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  for (int8_t i = 0; i < 4; ++i)
  {
    if ( userScaleSetup.application[i] == 1)
    {
      switch (i)
      {
        case 0: temp += "0-Weighing Mode\n"; break;
        case 1: temp += "1-Counting Mode\n"; break;
        case 2: temp += "2-Check Weighing\n"; break;
        case 3: temp += "3-Price Weighing\n"; break;
        default :  temp = "None"; break;
      }
    }
  }
  settingFile.close();

  return temp;
}


int8_t defaultSettings:: _setApplication(char *application)
{
  if ( application ) return -1;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");

  strcpy( userScaleSetup.application, application);

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}


String defaultSettings:: _getDate()
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  if (!settingFile)
  {
    Serial.println("\n_getDate() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  temp = userScaleSetup.dateString;

  settingFile.close();
  return temp;
}



int8_t defaultSettings:: _setDate(char *date)
{

  if ( !date ) return -1;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");

  strcpy(userScaleSetup.dateString, date );

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}




String defaultSettings:: _getAutoZero()
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  if (!settingFile)
  {
    Serial.println("\n _getAutoZero() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  idx = userScaleSetup.autoZero;

  switch (idx)
  {
    case 0: temp = "0 OFF"; break;
    case 1: temp = "1 Div"; break;
    case 2: temp = "2 Div"; break;
    case 3: temp = "3 Div"; break;
    case 4: temp = "4 Div"; break;
    case 5: temp = "5 Div"; break;
    default: temp = "none"; break;
  }

  settingFile.close();
  return temp;
}



int8_t defaultSettings:: _setAutoZero( int8_t autoZero )
{
  if ( autoZero >= 6 ) return -1;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");

  userScaleSetup.autoZero  = autoZero;

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}





String defaultSettings:: _getStability()
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  if (!settingFile)
  {
    Serial.println("\n _getAutoZero() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  idx = userScaleSetup.stabilityFactor;

  switch (idx)
  {
    case 0: temp = "Low"; break;
    case 1: temp = "Medium"; break;
    case 2: temp = "Normal"; break;
    case 3: temp = "High"; break;
    case 4: temp = "Strong"; break;
    default: temp = "none"; break;
  }

  settingFile.close();
  return temp;
}



int8_t defaultSettings:: _setStability( int8_t stability )
{
  if ( stability >= 6 ) return -1;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");

  userScaleSetup.stabilityFactor  = stability;

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}



String defaultSettings:: _getCOMPara( uint8_t COMPara)
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  if (!settingFile)
  {
    Serial.println("\n _getCOMPara() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  idx = userScaleSetup.comPara[ COMPara ];
  Serial.println("comPara : " + String( userScaleSetup.comPara[ COMPara ] ) );

  if ( COMPara == BAUDRATE )
  {
    switch (idx)
    {
      case 0: temp = "2400"  ; break;
      case 1: temp = "4800"  ; break;
      case 2: temp = "9600"  ; break;
      case 3: temp = "115200"; break;
      default: temp = "none" ; break;
    }
  }
  else if ( COMPara == DataBits )
  {
    switch (idx)
    {
      case 0: temp = "8 bits"; break;
      case 1: temp = "7 bits"; break;
      default: temp = "none"; break;
    }
  }
  else if ( COMPara == Parity )
  {
    switch (idx)
    {
      case 0: temp = "None"; break;
      case 1: temp = "odd"; break;
      case 2: temp = "Even"; break;
      default: temp = "none"; break;
    }
  }
  else if ( COMPara == StopBits )
  {
    switch (idx)
    {
      case 0: temp = "1 bit"; break;
      case 1: temp = "2 bits"; break;
      default: temp = "none"; break;
    }
  }

  settingFile.close();
  return temp;
}




String defaultSettings:: _setCOMPara( uint8_t COMPara,   int8_t idx )
{
  String temp;

  settingFile = SPIFFS.open(userSettingFilePath, FILE_READ);
  if (!settingFile)
  {
    Serial.println("\n _getAutoZero() : Error while opening file . ");
    return temp;
  }

  idx = userScaleSetup.comPara[COMPara];

  if ( COMPara == BAUDRATE )
  {
    switch (idx)
    {
      case 0: temp = "2400"; break;
      case 1: temp = "4800"; break;
      case 2: temp = "9600"; break;
      case 3: temp = "115200"; break;
      default: temp = "none"; break;
    }
  }
  else if ( COMPara == DataBits )
  {
    switch (idx)
    {
      case 0: temp = "8 bits"; break;
      case 1: temp = "7 bits"; break;
      default: temp = "none"; break;
    }
  }
  else if ( COMPara == Parity )
  {
    switch (idx)
    {
      case 0: temp = "None"; break;
      case 1: temp = "odd"; break;
      case 2: temp = "Even"; break;
      default: temp = "none"; break;
    }
  }
  else if ( COMPara == StopBits )
  {
    switch (idx)
    {
      case 0: temp = "1 bit"; break;
      case 1: temp = "2 bits"; break;
      default: temp = "none"; break;
    }
  }

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();
  return temp;
}




String defaultSettings:: _getWeighingUnit()
{
  String temp;
  int8_t idx = 0;
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, FILE_READ);

  if (settingFile <= 0)
  {
    Serial.println("\n _getWeighingUnit() : Error while opening file . ");
    return temp;
  }

  //  Serial.println("read : " + String( settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ))) );
  ( settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup )));
  for (idx = 0; !( userScaleSetup.WeighingUnits[idx] ); ++idx );


  switch (idx)
  {
    case 0: temp = "kg"; break;
    case 1: temp = "gm"; break;
    case 2: temp = "Tons"; break;
    case 3: temp = "Ounces(oz)"; break;
    case 4: temp = "Pounds(lb)"; break;
    default: temp = "none"; break;
  }

  settingFile.close();
  return temp;
}



int8_t defaultSettings:: _setWeighingUnit( int8_t unit )
{
  if ( unit >= 5 ) return -1;
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  if (settingFile <= 0)
    Serial.println("Error/open : _setWeighingUnit...!!!");

  for (int8_t idx = 0; idx < 5; idx++ )
    userScaleSetup.WeighingUnits[idx]  = 0;

  userScaleSetup.WeighingUnits[ unit ]  = 1;

  uint16_t writtenBytes = settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
  Serial.println("Total bytes written in SPIFFS memory : " + String(writtenBytes));

  settingFile.close();

  return  1;
}


int8_t defaultSettings::factoryDefault()
{
  settingFile =  SPIFFS.open(userSettingFilePath, "r+");
  settingFile.write((byte *)&ScaleSetup, sizeof(ScaleSetup));
  settingFile.close();
  return 1;
}

int8_t  defaultSettings::saveAsDefault()
{
  int8_t cnt = 0;
  while ( !cnt )
  {
    settingFile =  SPIFFS.open(userSettingFilePath, "r+" );

    if (settingFile <= 0)
    {
      Serial.println("Error : saveAsDefault()\nFile Cann't open");
      return -1;
    }

    cnt =  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
    Serial.println( "save as default -->>> " + String(cnt) );
    delay(100);
    settingFile.close();
    /*
       NOTE : if you are getting multifile times cnt value equals to zero.
              Please erase your whole chip using esptool.py. It misght work then.
    */
  }
  return 1;
}

String  defaultSettings::_getCapacity()
{
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));
  String temp = userScaleSetup.capacity;
  settingFile.close();
  return temp;
}



int8_t defaultSettings::_setCapacity( char *capacity )
{
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  strcpy( userScaleSetup.capacity, capacity );
  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
  settingFile.close();
  return  1;
}


String defaultSettings::_getCalibrationWeight()
{
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));
  String temp = userScaleSetup.calibartionWeight;
  settingFile.close();
  return  temp;
}

int8_t defaultSettings::_setCalibrationWeight(char *caliWeight )
{
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  strcpy( userScaleSetup.calibartionWeight, caliWeight );
  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
  settingFile.close();
  return  1;
}

String defaultSettings::_getEvalue()
{
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));
  String temp = String( userScaleSetup.eVlaue );
  settingFile.close();
  return  temp;
}
int8_t defaultSettings::_setEvalue( char eVlaue )
{
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  userScaleSetup.eVlaue[0] = eVlaue;
  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
  settingFile.close();
  return  1;
}

String defaultSettings::_getDecimal()
{
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));
  String temp = String( userScaleSetup.decimal );
  settingFile.close();
  return temp;
}

int8_t defaultSettings::_setDecimal( char decimal )
{
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  userScaleSetup.decimal[0] = decimal;
  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
  settingFile.close();
  return  1;
}

String defaultSettings::_getTime() {
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));
  String temp = String( userScaleSetup.timeArr );
  settingFile.close();
  return temp;

}
int8_t defaultSettings::_setTime() {

  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, "r+");

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
  settingFile.close();
  return  1;
}

int8_t  defaultSettings::_setPassword() {
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));
  settingFile.close();
  return  1;
}

void defaultSettings::_getPassword() {
  File settingFile;
  settingFile = SPIFFS.open(userSettingFilePath, "r+");
  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));
  // String temp = String( userScaleSetup.password );
  settingFile.close();
  // return  temp;
}



String  defaultSettings::_getHeader(int8_t idx ) {

  if ( idx >= 5 ) {
    return "Error: Invalid index, valid Index[0..4]\n";
  }
  File settingFile;
  String temp = "";
  settingFile = SPIFFS.open(userSettingFilePath, FILE_READ );
  if (!settingFile)
  {
    Serial.println("\n_getHeader() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  temp += userScaleSetup.saveHeader[idx];
  return temp;
}



String  defaultSettings::_getFooter(int8_t idx ) {

  if ( idx >= 3 ) {
    return "Error: Invalid index, valid Index[0..2]\n";
  }
  File settingFile;
  String temp = "";
  settingFile = SPIFFS.open(userSettingFilePath, FILE_READ );
  if (!settingFile)
  {
    Serial.println("\n_getFooter() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  temp += userScaleSetup.saveFooter[idx];
  return temp;
}
