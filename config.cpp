#include <EEPROM.h>
#include "config.h"

#define EEPROM_SIZE 512
#define MAX_SSID_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_DEVICE_NAME_LENGTH 50

struct ConfigData {
  char ssid[MAX_SSID_LENGTH];
  char password[MAX_PASSWORD_LENGTH];
  char deviceName[MAX_DEVICE_NAME_LENGTH];
  char uuid_list[];
};

ConfigData configData;

void loadConfigData() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, configData);
  EEPROM.end();
}

void saveConfigData() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(0, configData);
  EEPROM.commit();
  EEPROM.end();
}
