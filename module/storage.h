#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <Arduino.h>
#include <EEPROM.h>

#include "type.h"

class Storage{
    public:
        void set(eeprom_data_list_t*);
        eeprom_data_list_t* get();
        void clear();
        void release(eeprom_data_list_t*);
};

#endif
