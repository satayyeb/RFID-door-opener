#pragma once

#include <EEPROM.h>
#include <Arduino.h>
#include <ArduinoJSON.h>
#include <ESP8266WiFi.h>


#define DEFAULT_SSID "esp"
#define DEFAULT_PASS "asdfghjk"
#define DEFAULT_LOGIN_USER "admin"
#define DEFAULT_LOGIN_PASS "admin"
#define DEFAULT_IP IPAddress(192, 168, 1, 1)


class Settings {
private:
    const int settings_string_size = 300;
public:
    IPAddress subnet = IPAddress(255, 255, 255, 0);
    IPAddress ip;
    String wifi_ssid;
    String wifi_pass;
    String login_user;
    String login_pass;

    Settings();
    void write_defaults_into_eeprom();
    void load_settings_from_eeprom();
    void update_settings_in_eeprom();
};

