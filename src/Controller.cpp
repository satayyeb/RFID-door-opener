#include "Controller.h"


Controller::Controller() {

}

Controller::Controller(Settings* settings, Library* library) {
    this->settings = settings;
    this->library = library;
}

void Controller::log(String log) {
    Serial.println(log);
    logs.push(log);
    while (logs.size() >= 20)
        logs.pop();
}

String Controller::get_last_log() {
    if (logs.empty())
        return "No log";
    return logs.back();
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