#pragma once

#include <SPI.h>
#include <MFRC522.h>
#include "hardwareConfig.h"
#include "Card.h"



class Library {
private:
    MFRC522::MIFARE_Key mifare_key;
    MFRC522::StatusCode mifare_status;
    std::vector<Card> cards;
    int sectors[16] = { 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63 };
    bool write_key_on_card(Card* card);
    bool update_cards_on_eeprom();
public:
    MFRC522 rfid;
    Library();
    std::vector<Card> getCards();
    bool is_modifying = false;
    bool check_card();
    bool reset_card();
    bool add_card(Card* card);
    bool remove_card(int index);
    void remove_all_cards();
    Card get_card_by_scan();
};

