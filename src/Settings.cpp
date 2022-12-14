#include "Settings.h"
#include "GlobalObjects.h"


Settings::Settings() {
    EEPROM.begin(4096);
    //for debug:
    // Controller::dumpEEPROM(2048);
    if (EEPROM.read(0) != '{') {
        Logger.log("No system settings found on EEPROM. Writing defaults on it...");
        write_defaults_into_eeprom();
    }
    load_settings_from_eeprom();
}

void Settings::load_settings_from_eeprom() {
    Logger.log("Loading system settings from EEPROM...");
    char json_string[300];
    EEPROM.get(0, json_string);
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, json_string);
    if (error != DeserializationError::Ok) {
        Logger.log("An error occurred during deserializing system settings. error: " + String(error.f_str()));
        exit(-1);
    }
    String ali = "lnasdk";
    ip.fromString(String(doc["ip"]));
    wifi_ssid = String(doc["wifi_ssid"]);
    wifi_pass = String(doc["wifi_pass"]);
    login_user = String(doc["login_user"]);
    login_pass = String(doc["login_pass"]);
}

void Settings::write_defaults_into_eeprom() {
    StaticJsonDocument<300> doc;
    doc["ip"] = DEFAULT_IP;
    doc["wifi_ssid"] = DEFAULT_SSID;
    doc["wifi_pass"] = DEFAULT_PASS;
    doc["login_user"] = DEFAULT_LOGIN_USER;
    doc["login_pass"] = DEFAULT_LOGIN_PASS;
    char json_string[300] = { '\0' };
    serializeJson(doc, json_string, sizeof(json_string));
    EEPROM.put(0, json_string);
    EEPROM.commit();
}

void Settings::update_settings_in_eeprom() {
    char json_string[300] = { '\0' };
    StaticJsonDocument<300> doc;
    doc["ip"] = ip;
    doc["wifi_ssid"] = wifi_ssid;
    doc["wifi_pass"] = wifi_pass;
    doc["login_user"] = login_user;
    doc["login_pass"] = login_pass;
    serializeJson(doc, json_string, sizeof(json_string));
    EEPROM.put(0, json_string);
    EEPROM.commit();
}

