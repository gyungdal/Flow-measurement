#ifndef __TEST_H__
#define __TEST_H__

#include "storage.h"
#include "type.h"

class Test{
    public:
        Test();
        bool eeprom();
    private:
        eeprom_data_list_t* generate_eeprom_list();
};

#endif
