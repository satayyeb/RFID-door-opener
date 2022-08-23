#pragma once

#include <Arduino.h>
#include <ArduinoJSON.h>
#include <EEPROM.h>

class Card {
private:
public:
    String card_name;
    byte key[12];
    int sector;
    Card();
    Card(String card_name);
    Card(const char* card_name, JsonArray key, int sector);
    bool operator == (const Card& obj) const;
};


