#include "storage.h"

void Storage::clear(){
    #ifdef DEBUG
        Serial.println("[INFO] EEPROM Clear Start!");
    #endif
    for(int i = 0;i<EEPROM.length();i++){
        EEPROM.write(i, 0);
    }
    #ifdef DEBUG
        Serial.println("[INFO] EEPROM Clear Done!");
    #endif
}

void Storage::saveSetting(eeprom_setting_t* setting){ 
    auto eepromOffset = (EEPROM.length() - 1 - sizeof(eeprom_setting_t));
    uint8_t* serializeBytes = new uint8_t[sizeof(eeprom_setting_t)];
    #ifdef DEBUG
        Serial.print("[INFO] Save Setting");
        Serial.print(" Size : ");
        Serial.println(sizeof(eeprom_setting_t));
    #endif
    memcpy(serializeBytes, setting, sizeof(eeprom_setting_t));
    for(auto i = 0;i < sizeof(eeprom_setting_t);i++){
        EEPROM.write(i + eepromOffset, serializeBytes[i]);
    }
    delete[] serializeBytes;
}

eeprom_setting_t* Storage::readSetting(){
    auto eepromOffset = (EEPROM.length() - 1 - sizeof(eeprom_setting_t));
    eeprom_setting_t* setting = new eeprom_setting_t;
    uint8_t* serializeBytes = calloc(sizeof(uint8_t), sizeof(eeprom_setting_t));
    for(int i = 0;i<sizeof(eeprom_setting_t);i++){
        serializeBytes[i] = EEPROM.read((eepromOffset + i));
    }
    memcpy(setting, serializeBytes, sizeof(eeprom_setting_t));
    q32delete[] serializeBytes;
    return setting;
}

void Storage::set(eeprom_list_t* data){
    uint16_t bufferSize = (sizeof(eeprom_item_t) * data->length) + sizeof(uint8_t);

    #ifdef DEBUG
    Serial.print("[INFO] Write Buffer Size : ");
    Serial.println(bufferSize);
    #endif
    
    uint8_t* serializeBytes = new uint8_t[bufferSize];
    serializeBytes[0] = data->length;
    for(uint8_t i = 0;i<data->length;i++){
        memcpy(&serializeBytes[(i * sizeof(eeprom_item_t)) + 1], &data->items[i], sizeof(eeprom_item_t));
    }
    for(uint8_t i = 0;i<((sizeof(eeprom_item_t) * data->length) + sizeof(uint8_t));i++){
        EEPROM.write(i, serializeBytes[i]);
    }
    delete[] serializeBytes;
}


eeprom_list_t* Storage::get(){
    eeprom_list_t* result = new eeprom_list_t;
    result->items = nullptr;
    result->length = EEPROM.read(0);
    #ifdef DEBUG
    uint16_t bufferSize = 1;
    Serial.print("[INFO] GET LENGTH ");
    Serial.println(result->length);
    #endif
    if(result->length != 0){
        result->items = new eeprom_item_t[result->length];
        uint8_t* buffer = new uint8_t[sizeof(eeprom_item_t)];
        for(uint8_t i = 0;i<result->length;i++){
            memset(buffer, 0x00, sizeof(eeprom_item_t));
            for(uint8_t j = 0;j<sizeof(eeprom_item_t);j++){
                buffer[j] = EEPROM.read(((sizeof(eeprom_item_t) * i) + j + 1));
                #ifdef DEBUG
                bufferSize++;
                #endif
            }
            memcpy(&result->items[i], buffer, sizeof(eeprom_item_t));
        #ifdef DEBUG
        Serial.print("[INFO] READ INDEX");
        Serial.println(i);
        #endif
        }
        #ifdef DEBUG
        Serial.print("[INFO] Read Buffer Size : ");
        Serial.println(bufferSize);
        #endif
        delete[] buffer;
    }
    return result;
}

void Storage::release(eeprom_list_t* data){
    delete[] data->items;
    delete data;
}
