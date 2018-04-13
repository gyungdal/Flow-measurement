#include "type.h"

eeprom_liquid_amount_t literToAmount(double liter){
    eeprom_liquid_amount_t result;
    result.liter = ((uint32_t)liter / 1000);
    result.mill_liter = ((uint16_t)liter % 1000);
    return result;
}