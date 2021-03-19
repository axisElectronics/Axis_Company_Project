
#include "defaultSettings.h"

void initEEPROM()
{
  // initialize EEPROM with predefined size
  if ( !EEPROM.begin(EEPROM_SIZE) )
  {
    Serial.println("failed to initialise EEPROM");
    delay(5000);
  }

}

void setDefaultSettings()
{
  // 1. initialise EEPROM
  initEEPROM();
  // 2. Save default Application & Weighing Unit

  EEPROM.write( WeighingApplicationAddress, ( defaultApplication | defaultWeighingUnit ) );
  EEPROM.write( COMPortSettingAddress, ( defaultbaudRate | defaultDataBit | defaultStopBit | defaultParity ) );


  EEPROM.commit();

}

#define EEPROMWrite( Address, Data )   if ( !EEPROM.begin(EEPROM_SIZE) )  \
  { \
    Serial.println("failed to initialise EEPROM"); \
    delay(5000); \
  } \
  EEPROM.write( Address,  ( Data ) ); \
  EEPROM.commit();


int8_t _setBuadrate(char *baudrate)
{
  int8_t temp;
  if ( !strcmp( baudrate, "2400") )       temp = 0;
  else if ( !strcmp(baudrate, "4800") )   temp = 1;
  else if ( !strcmp(baudrate, "9600") )   temp = 2;
  else if ( !strcmp(baudrate, "115200") ) temp = 3;
  else return -1;
  int Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) & ( 0xF0 | ( temp & 0x0F ) ) );
  EEPROMWrite( COMPortSettingAddress , Data  );
  return temp;
}




String _getBuadrate()
{
  String baudrate = String(NULL);
  int8_t baudBit = ( byte( EEPROM.read( COMPortSettingAddress ) ) & 0x0F );
  switch ( baudBit )
  {
    case 0 : baudrate += "2400"; break;
    case 1 : baudrate += "4800"; break;
    case 2 : baudrate += "9600"; break;
    case 3 : baudrate += "115200"; break;
    default :
      baudrate += "9600";
      break;
  }
  return baudrate;
}


int8_t _getDataBits()
{
  int8_t temp = ( byte( EEPROM.read( COMPortSettingAddress ) ) & 0x10 );
  if ( ( ( temp >> 4 ) & 0x01 ) ) return 8;
  else if ( !temp ) return 7;

  return -1;
}


int8_t  _setDataBits(int8_t DataBits )
{
  int8_t temp;
  if ( ( DataBits == 8 ) )
  {
    int Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) | ( 1 << 4 ) );
    EEPROMWrite( COMPortSettingAddress , Data  );
    return 8;
  }
  else if ( ( DataBits == 7 ) )
  {
    int Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) | ( 0 << 4 ) );
    EEPROMWrite( COMPortSettingAddress , Data  );
    return 7;
  }

  return -1;
}

int8_t _getStopBits()
{
  int8_t temp = ( byte( EEPROM.read( COMPortSettingAddress ) ) & 0x20 );
  if ( ( ( temp >> 5 ) & 0x01 ) ) return 2;
  else if ( !temp ) return 1;

  return -1;
}


int8_t  _setStopBits(int8_t StopBits )
{

  if ( ( StopBits == 1 ) )
  {
    int Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) | ( 0 << 5 ) );
    EEPROMWrite( COMPortSettingAddress , Data  );
    return 1;
  }
  else if ( ( StopBits == 2 ) )
  {
    int Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) | ( 1 << 5 ) );
    EEPROMWrite( COMPortSettingAddress , Data  );
    return 2;
  }

  return -1;
}

String _getParityBits()
{
  uint8_t temp = ( byte( EEPROM.read( COMPortSettingAddress ) ) & 0xC0 );

  if ( !( ( temp >> 6 ) & 0x03 ) ) return "None";
  else if ( ( ( temp >> 6 ) & 0x03 ) == 1 ) return "odd";
  else if ( ( ( temp >> 6 ) & 0x03 ) == 2 ) return "Even";
  return "Error";
}


int8_t  _setParityBits(char *ParityBits )
{
  int8_t temp = 0;

  if ( !strcmp( ParityBits, "None" ) )
  {
    int8_t Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) | ( 0 << 6 ) );
    EEPROMWrite( COMPortSettingAddress , Data  );
    return 0;
  }
  else if ( !strcmp( ParityBits, "odd") )
  {
    int8_t Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) | ( 1 << 6 ) );
    EEPROMWrite( COMPortSettingAddress , Data  );
    return 1;
  }
  else if ( !strcmp( ParityBits, "Even") )
  {
    int8_t Data = ( ( byte( EEPROM.read( COMPortSettingAddress ) ) ) | ( 2 << 6 ) );
    EEPROMWrite( COMPortSettingAddress , Data  );
    return 2;
  }


  return -1;
}



String _getDate()
{
  String date = "";
  date += String( byte( EEPROM.read( dateAddress ) ) );
  date += '/';
  date += String( byte( EEPROM.read( monthAddress) ) );
  date += '/';
  date += String( byte( EEPROM.read( yearAdressH ) ) );
  date += String( byte( EEPROM.read( yearAdressL ) ) );

  return date;
}

char *_setDate(char *date )
{
  int8_t temp = 0;
  temp = ( 10 * ( date[0] - 48 ) ) + ( date[1] - 48 );
  EEPROMWrite( dateAddress , temp  );

  temp = ( 10 * ( date[3] - 48 ) ) + ( date[4] - 48 );
  EEPROMWrite( monthAddress , temp  );

  temp = ( 10 * ( date[6] - 48 ) ) + ( date[7] - 48 );
  EEPROMWrite( yearAdressH , temp  );

  temp = ( 10 * ( date[8] - 48 ) ) + ( date[9] - 48 );
  EEPROMWrite( yearAdressL , temp  );

  return date;
}



int8_t _geteValue()
{
  int8_t temp;
  switch ( byte( EEPROM.read( eValueAddress ) ) )
  {
    case 1 : temp = 1;  break;
    case 2 : temp = 2;  break;
    case 5 : temp = 5;  break;
    default :  temp = -1;  break;
  }
  return temp;
}



int8_t _seteValue( int8_t eValue )
{
  EEPROMWrite( eValueAddress , eValue  );
  return byte( EEPROM.read( eValueAddress ) );
}

int8_t _getdecimal()
{
  int8_t temp;
  switch ( byte( EEPROM.read( decimalAddress ) ) )
  {
    case 1 : temp = 1;  break;
    case 2 : temp = 2;  break;
    case 3 : temp = 3;  break;
    case 4 : temp = 4;  break;
    case 5 : temp = 5;  break;
    default :  temp = -1;  break;
  }
  return temp;
}



int8_t _setdecimal(int8_t decimal )
{
  EEPROMWrite( decimalAddress , decimal  );
  return byte( EEPROM.read( decimalAddress ) );
}



char *_setCapacity( char *capacity )
{
  int8_t len = strlen(capacity);
  Serial.println("temp len : " + String(len ));
  EEPROMWrite( capacityAddress , len );

  for( int8_t i = 0; i < len; i++)
  {
     EEPROMWrite( ( capacityAddress + (i + 1) ) , capacity[i] );
  }

  return capacity;
}

String _getCapacity( )
{
  int8_t len = byte( EEPROM.read( capacityAddress ) );
  String capacity = "";
  Serial.println("len : " + String(len ));

  for( int8_t i = 0; i < len; i++ )
  {
    capacity += ( char( EEPROM.read( capacityAddress+i+1 ) ) );
  }

  return capacity;
}
