#include "Controller.h"


Controller::Controller(Settings* settings, Library* library, Logger* logger) {
    this->settings = settings;
    this->library = library;
    this->logger = logger;
}


void Controller::reboot() {
    ESP.restart();
}


void Controller::open_door() {
    digitalWrite(DOOR_RELAY, HIGH);
    delay(400);
    digitalWrite(DOOR_RELAY, LOW);
}


void Controller::factory_reset() {
    settings->write_defaults_into_eeprom();
    library->remove_all_cards();
    reboot();
}


void Controller::dumpEEPROM(int size) {
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