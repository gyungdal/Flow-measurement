#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <Arduino.h>
#include <EEPROM.h>

#include "type.h"

class Storage{
    public:
        #ifdef TEST_EEPROM
            void testValue();
        #endif
        void set(eeprom_list_t*);
        eeprom_list_t* get();
        void clear();
        void clearHistory();
        void release(eeprom_list_t*);
        void saveSetting(eeprom_setting_t*);
        eeprom_setting_t* readSetting();
};

#endif
