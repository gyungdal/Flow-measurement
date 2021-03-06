#include "storage.h"

#define DEBUG

#ifdef TEST_EEPROM
void Storage::testValue(){
    randomSeed(analogRead(A0));
    eeprom_list_t* sample = new eeprom_list_t;
    sample->length = 30;
    sample->items = new eeprom_item_t[sample->length];
    for(uint8_t i = 0;i<sample->length;i++){
        eeprom_item_t* item = new eeprom_item_t;
        item->index = i;
        item->time.year = 2018;
        item->time.month = random(1, 13);
        
        item->time.day = i + 1;
        item->amount.liter = random(0, 3000);
        item->amount.milliLiter = random(0, 1000);
        memcpy(&sample->items[i], item, sizeof(eeprom_item_t));
        delete item;
    }
    set(sample);
    Serial.println("SAMPLE DONE");
    delete[] sample->items;
    delete sample;
    Serial.println("EEPROM DONE");
}        
#endif

void Storage::clear(){
    #ifdef DEBUG
        Serial.println("[INFO] EEPROM Clear Start!");
    #endif
    for(uint16_t i = 0;i<EEPROM.length();i++){
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
    for(uint16_t i = 0;i < sizeof(eeprom_setting_t);i++){
        EEPROM.write(i + eepromOffset, serializeBytes[i]);
    }
    delete[] serializeBytes;
}

eeprom_setting_t* Storage::readSetting(){
    auto eepromOffset = (EEPROM.length() - 1 - sizeof(eeprom_setting_t));
    if(EEPROM.read(eepromOffset) == 0){
        return nullptr;
    }
    uint8_t* serializeBytes = (uint8_t*)calloc(sizeof(uint8_t), sizeof(eeprom_setting_t));
    for(uint16_t i = 0;i<sizeof(eeprom_setting_t);i++){
        serializeBytes[i] = EEPROM.read((eepromOffset + i));
    }
    eeprom_setting_t* setting = new eeprom_setting_t;
    memcpy(setting, serializeBytes, sizeof(eeprom_setting_t));
    delete[] serializeBytes;
    return setting;
}

void Storage::clearHistory(){
    uint8_t length = EEPROM.read(0);
    EEPROM.write(0, 0);
    for(uint16_t i = 0;i<length;i++){
        for(uint16_t j = 0;j<sizeof(eeprom_item_t);j++){
            EEPROM.write((sizeof(eeprom_item_t) * i) + j + 1, 0);
        }
    }
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
    #ifdef DEBUG
    Serial.print("[INFO] Memcpy Buffer");
    Serial.println(bufferSize);
    #endif
    for(uint16_t i = 0;i<bufferSize;i++){
        EEPROM.write(i, serializeBytes[i]);
        #ifdef DEBUG
        Serial.print("[INFO] Write");
        Serial.println(i);
        #endif
    }
    #ifdef DEBUG
    Serial.print("[INFO] Write eeprom");
    Serial.println(bufferSize);
    #endif
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
        for(uint16_t i = 0;i<result->length;i++){
            memset(buffer, 0x00, sizeof(eeprom_item_t));
            for(uint16_t j = 0;j<sizeof(eeprom_item_t);j++){
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
