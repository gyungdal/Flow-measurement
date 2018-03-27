#include "storage.h"

void setup_eeprom(){
    if((EEPROM.read(0) ^ EEPROM_XOR_VALUE)) != 0x00){
        for(int i = 0;i<EEPROM.length();i++){
            EEPROM.write(i, 0);
        }
    }
}

inline size_t get_eeprom_data_length(){
    size_t index = 0;
    
    while((EEPROM.read(index * 8) ^ EEPROM_XOR_VALUE) == index)
        index++;
    
    return index;
}

inline eeprom_data_item_t get_eeprom_data_item(int index){
    eeprom_data_item_t item;

    int offset = (index * sizeof(eeprom_data_item_t));
    if(EEPROM.read(offset) != index)
        return nullptr;

    uint8_t* temp = new uint8_t[sizeof(eeprom_data_item_t)];
    for(uint8_t i = 0;i<sizeof(eeprom_data_item_t);i++){
        temp[i] = (EEPROM.read(offset + i) ^ EEPROM_XOR_VALUE);
    }
    
    memcpy(&item, temp, sizeof(eeprom_data_item_t));
    
    delete[] temp;

    return item;
}
    
eeprom_data_list_t get_eeprom_data_list(){
    eeprom_data_list_t result; 
    uint8_t* data = new uint8_t[sizeof(eeprom_data_item_t)];
    eeprom_data_item_t item;
    while((item = get_eeprom_data_item(result.length) != nullptr){
        result.length += 1;
        realloc(result.items, sizeof(eeprom_data_item_t) * (result.length + 1));
        memcpy(&result.items[result.length], item, sizeof(eeprom_data_item_t));
    }
    return result;
}

inline void set_eeprom_data_item(int index, eeprom_data_item_t item){
    int offset = (index * sizeof(eeprom_data_item_t));

    uint8_t* bytes = new uint8_t[sizeof(eeprom_data_item_t)];
    memcpy(bytes, &item, sizeof(eeprom_data_item_t));
    for(uint8_t i = 0;i<sizeof(eeprom_data_item_t);i++){
        EEPROM.write(offset + i, (bytes[i] ^ EEPROM_XOR_VALUE));
    }
    delete[] bytes;
}

void set_eeprom_data_list(eeprom_data_list_t items){
    for(uint8_t i = 0;i<items.length;i++){
        set_eeprom_data_item(i, items[i]);
    }
}


void release_eeprom_list(eeprom_data_list_t list){
    delete[] list.items;
    delete &list;
}

void release_eeprom_item(eeprom_data_item_t item){
    delete item;
}
