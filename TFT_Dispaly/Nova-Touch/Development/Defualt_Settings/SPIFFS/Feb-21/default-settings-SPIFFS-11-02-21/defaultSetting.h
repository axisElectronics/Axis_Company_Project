#ifndef defaultSetting_h
#define defaultSetting_h

#include <FS.h>
#include <SPIFFS.h>

#define WeighingMode  0
#define CountingMode  1
#define CheckWeighing 2
#define PriceWeighing 3


#define BAUDRATE 0
#define DataBits    1
#define Parity      2
#define StopBits    3
#define OutputMode  4

#define kg     0
#define gm     1
#define tons   2
#define Ounces 3
#define Pounds 4


#define developer       0
#define baseVersion     1
#define modifiedVersion 2




class defaultSettings
{
  public :

    struct defaultSetting_ScaleSetup
    {
      // 1. Application Setup
      char application[4];

      // 2. scale setup
      char dateString[10];
      int8_t autoZero;
      int8_t stabilityFactor;
      char comPara[5];

      //3. Weighing Units
      char WeighingUnits[5];

      //4. scale information
      char scaleInformation[3][15];

      //5. password
      char password[20];

      //6. factory reset


    } ScaleSetup, userScaleSetup;


    defaultSettings()
    {

    }
    void updateDefaultSettings( );
    int8_t initDefaultSettings( );

    String _getApplication();
    int8_t _setApplication(int8_t application);

    String _getDate();
    int8_t _setDate(char *date);

    String _getAutoZero();
    int8_t _setAutoZero( int8_t autoZero );

    String  _getStability();
    int8_t  _setStability( int8_t stability );

    String  _getCOMPara( uint8_t COMPara);
    String  _setCOMPara( uint8_t COMPara,   int8_t idx );

    String _getWeighingUnit();
    int8_t _setWeighingUnit( int8_t unit );

    int8_t factoryDefault();
    int8_t saveAsDefault();


};


#endif
