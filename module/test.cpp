#include "test.h"

Test::Test(){
    Serial.println("===============================================");
    Serial.println("\t\t [DEBUG MODE]");
    Serial.println("===============================================");
}

bool Test::eeprom(){
    long time = millis();
    Serial.println("[START] EEPROM TEST");
    eeprom_data_list_t* generate = this->generate_eeprom_list();
    Serial.print("[USE TIME] ");
    Serial.println(millis() - time);
    time = millis();

    Serial.println("[START] SET EEPROM");
    set_eeprom_data_list(generate);
    Serial.println("[SUCCESS] SET EEPROM");
    Serial.print("[USE TIME] ");
    Serial.println(millis() - time);
    time = millis();

    Serial.println("[START] GET EEPROM");
    eeprom_data_list_t* eepromData = get_eeprom_data_list();
    Serial.println("[SUCCESS] GET EEPROM");
    Serial.print("[USE TIME] ");
    Serial.println(millis() - time);
    time = millis();
    
    Serial.print("[MATCH] ");
    Serial.println(memcmp(generate->items, eepromData->items, (sizeof(eeprom_data_item_t) * generate->length)));
    
    if(memcmp(generate->items, eepromData->items, (sizeof(eeprom_data_item_t) * generate->length)) != 0){
        Serial.println("[RESULT] EERPOM TEST FAIL!");
    }else{
        Serial.println("[RESULT] EEPROM TEST SUCCESS");
    }
    Serial.print("[USE TIME] ");
    Serial.println(millis() - time);
    
    release_eeprom_list(generate);   
    release_eeprom_list(eepromData); 
}

eeprom_data_list_t* Test::generate_eeprom_list(){
    Serial.println("[START] Generate Sample List");
    eeprom_data_list_t* sample = new eeprom_data_list_t;
    sample->length = random(1, 10);
    sample->items = new eeprom_data_item_t[sample->length];
    for(uint8_t i = 0;i<sample->length;i++){
        eeprom_data_item_t item;

        item.index = i;

        item.time.year = 2018;
        
        item.time.month = random(1, 13);
        
        item.time.day = random(1, 31);
        item.time.hour = random(1, 13);
        item.time.am_pm = random(0, 2);
        
        item.time.minute = random(0, 60);
        
        item.time.second = random(0, 60);

        item.amount.liter = random(0, 3000);
        item.amount.mill_liter = random(0, 1000);
        memcpy(&sample->items[i], &item, sizeof(eeprom_data_item_t));
    }
    Serial.println("[SUCCESS] Generate Sample List");
    return sample;
}
