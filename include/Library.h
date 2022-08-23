#pragma once

#include <SPI.h>
#include <MFRC522.h>
#include "Card.h"
#include "hardwareConfig.h"
#include "Logger.h"



class Library {
private:
    Logger* logger;
    MFRC522::MIFARE_Key mifare_key;
    MFRC522::StatusCode mifare_status;
    std::vector<Card> cards;
    int sectors[16] = { 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63 };
    bool write_key_on_card(Card card);
    bool update_cards_on_eeprom();
public:
    MFRC522 rfid;
    Library(Logger* logger);
    std::vector<Card> getCards();
    bool check_card();
    bool reset_card(Card card);
    bool add_card(Card card);
    bool remove_card(int index);
    void remove_all_cards();
};


