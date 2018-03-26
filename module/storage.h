#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <EEPROM.h>
#include "type.h"

inline eeprom_data_item_t get_eeprom_data_item(int);
eeprom_data_list_t get_eeprom_data_list();

inline void set_eeprom_data_item(int, eeprom_data_item_t);
void set_eeprom_data_list(eeprom_data_list_t);
#endif