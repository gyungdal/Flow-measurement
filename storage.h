#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <Arduino.h>
#include <EEPROM.h>

#include "type.h"

void clear_eeprom();

void set_eeprom_data_list(eeprom_data_list_t*);
eeprom_data_list_t* get_eeprom_data_list();

void release_eeprom_list(eeprom_data_list_t*);

#endif
