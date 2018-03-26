#include "storage.h"


inline eeprom_data_item_t get_eeprom_data_item(int index){
    eeprom_data_item_t item;

    int offset = (index * sizeof(eeprom_data_item_t));
    if(EEPROM.read(offset) != index)
        return nullptr;

    uint8_t* temp = new uint8_t[sizeof(eeprom_data_item_t)];
    for(uint8_t i = 0;i<sizeof(eeprom_data_item_t);i++){
        temp[i] = EEPROM.read(offset + i);
    }
    
    memcpy(&item, temp, sizeof(eeprom_data_item_t));
    
    delete temp;

    return item;
}
    
eeprom_data_list_t get_eeprom_data_list(){
    eeprom_data_list_t result; 
    uint8_t* data = new uint8_t[sizeof(eeprom_data_item_t)];
    eeprom_data_item_t item;
    while((item = get_eeprom_data_item(result.length) != nullptr){
        ++result.length;
        realloc(result.items, sizeof(eeprom_data_item_t) * result.length + 1);
    }
    return result;
}

inline void set_eeprom_data_item(int index, eeprom_data_item_t item){
    int offset = (index * sizeof(eeprom_data_item_t));

    uint8_t* bytes = new uint8_t[sizeof(eeprom_data_item_t)];
    memcpy(bytes, &item, sizeof(eeprom_data_item_t));
    for(uint8_t i = 0;i<sizeof(eeprom_data_item_t);i++){
        EEPROM.write(offset + i, bytes[i]);
    }
}

void set_eeprom_data_list(eeprom_data_list_t items){
    for(uint8_t i = 0;i<items.length;i++){
        set_eeprom_data_item(i, items[i]);
    }
}
