
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


String defaultSettings:: _getDate()
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open("/userConfigfile.txt", FILE_READ);
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

  settingFile = SPIFFS.open("/userConfigfile.txt", "w+");

  strcpy(userScaleSetup.dateString, date );

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}




String defaultSettings:: _getAutoZero()
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open("/userConfigfile.txt", FILE_READ);
  if (!settingFile)
  {
    Serial.println("\n _getAutoZero() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  idx = userScaleSetup.autoZero;

  switch (idx)
  {
    case 0: temp = "off"; break;
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

  settingFile = SPIFFS.open("/userConfigfile.txt", "w+");

  userScaleSetup.autoZero  = autoZero;

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}





String defaultSettings:: _getStability()
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open("/userConfigfile.txt", FILE_READ);
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

  settingFile = SPIFFS.open("/userConfigfile.txt", "w+");

  userScaleSetup.stabilityFactor  = stability;

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}



String defaultSettings:: _getCOMPara( uint8_t COMPara)
{
  String temp;
  int8_t idx;

  settingFile = SPIFFS.open("/userConfigfile.txt", FILE_READ);
  if (!settingFile)
  {
    Serial.println("\n _getAutoZero() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  idx = userScaleSetup.comPara[COMPara];

  if ( COMPara == Baudrate )
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

  settingFile.close();
  return temp;
}




String defaultSettings:: _setCOMPara( uint8_t COMPara,   int8_t idx )
{
  String temp;

  settingFile = SPIFFS.open("/userConfigfile.txt", FILE_READ);
  if (!settingFile)
  {
    Serial.println("\n _getAutoZero() : Error while opening file . ");
    return temp;
  }

  idx = userScaleSetup.comPara[COMPara] = idx;

  if ( COMPara == Baudrate )
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
  int8_t idx;

  settingFile = SPIFFS.open("/userConfigfile.txt", FILE_READ);
  if (!settingFile)
  {
    Serial.println("\n _getAutoZero() : Error while opening file . ");
    return temp;
  }

  settingFile.read((byte *)&userScaleSetup, sizeof( userScaleSetup ));

  while ( !userScaleSetup.WeighingUnits[idx++] );

  switch (--idx)
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

  settingFile = SPIFFS.open("/userConfigfile.txt", "w+");

  for (int8_t idx = 0; idx < 5; idx++ )
    userScaleSetup.WeighingUnits[idx]  = 0;

  userScaleSetup.WeighingUnits[unit]  = 1;

  settingFile.write((byte *)&userScaleSetup, sizeof(userScaleSetup));

  settingFile.close();

  return  1;
}
