#include "Controller.h"


void reboot() {
    ESP.restart();
}


void open_door() {
    digitalWrite(DOOR_RELAY, HIGH);
    delay(400);
    digitalWrite(DOOR_RELAY, LOW);
}


void factory_reset() {
    settings.write_defaults_into_eeprom();
    library.remove_all_cards();
    reboot();
}


void dumpEEPROM(int size) {
    Serial.println("Dumping the eeprom:");
    byte ch;
    String str = "";
    for (int i = 0; i < size; i++) {
        ch = EEPROM.read(i);
        if (ch == 255)
            str += '\0';
        else
            str += char(ch);
    }
    Serial.println(str);
}