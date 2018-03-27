#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <Arduino.h>
#include <EEPROM.h>
#include <stdlib.h>

#include "type.h"

void setup_eeprom();

inline size_t get_eeprom_data_length();
inline eeprom_data_item_t get_eeprom_data_item(int);
eeprom_data_list_t get_eeprom_data_list();

inline void set_eeprom_data_item(int, eeprom_data_item_t);
void set_eeprom_data_list(eeprom_data_list_t);

void release_eeprom_list(eeprom_data_list_t);
void release_eeprom_item(eeprom_data_item_t);

#endif