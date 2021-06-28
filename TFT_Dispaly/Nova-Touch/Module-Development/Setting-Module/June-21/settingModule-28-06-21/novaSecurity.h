
#ifndef NOVA_SECURITY_H
#define NOVA_SECURITY_H

#include <iostream>
#include "settings.h"

enum {
  ERROR = -1,
  root = 0,
  admin,
  service
} User;


class novaSecurity {
  private :
    class defaultSettings spiffs;
    char _adminPassword[10], _servicePassword[10], _rootPassword[10];
    void _fetchPassword();
  public :
    char _tempPassword[10];
    int8_t enterPassword();
    int8_t validatePassword();
    void setAdminPassword();
};

 int8_t  novaSecurity:: enterPassword(){
  
 }

void novaSecurity:: setAdminPassword(char *setAdmin) {
  memset(spiffs.userScaleSetup.password[admin], '\0', sizeof(spiffs.userScaleSetup.password[admin] );
         strcpy(spiffs.userScaleSetup.password[admin], setAdmin);
         spiffs._setPassword();
}



void  novaSecurity:: _fetchPassword() {
  spiffs._getPassword();
}



int8_t novaSecurity:: validatePassword() {
  if ( strcmp(_tempPassword, _rootPassword ) ) {
    return root;
  }

  if ( strcmp(_tempPassword, _adminPassword ) ) {
    return admin;
  }

  if ( strcmp(_tempPassword, _servicePassword ) ) {
    return service;
  }
  return ERROR;
}

#endif
