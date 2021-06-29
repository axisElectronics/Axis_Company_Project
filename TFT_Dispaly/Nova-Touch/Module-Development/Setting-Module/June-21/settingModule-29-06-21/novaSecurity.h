
#ifndef NOVA_SECURITY_H
#define NOVA_SECURITY_H

#include <iostream>
#include "settings.h"

enum {
  TimeOutERROR = -2,
  ERROR = -1,
  root = 0,
  admin,
  service,
  minPasswordLength = 5,
  minPasswordRetryAttemped = 3,
} User;


class novaSecurity {
  private :
    class defaultSettings spiffs;
    class userKeyBoard kbd;
     
    char _adminPassword[10]="123456", _servicePassword[10], _rootPassword[10];
    int8_t faildAttemped;
    uint32_t reAttemped_TimeOut;
    void _fetchPassword();
  public :
    char _tempPassword[10];
    int8_t enterPassword();
    int8_t validatePassword();
    void setAdminPassword(char *setAdmin);
};


#endif
