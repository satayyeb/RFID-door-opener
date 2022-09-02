#include "Library.h"
#include "GlobalObjects.h"


Library::Library() {
    rfid = MFRC522(SS_MFRC, RST_MFRC);
    // Init MFRC522
    SPI.begin();
    rfid.PCD_Init();
    Logger.log("Loading cards from EEPROM...");
    char json_string[3000];
    EEPROM.get(600, json_string);
    StaticJsonDocument<3000> doc;
    DeserializationError error = deserializeJson(doc, json_string);
    if (error != DeserializationError::Ok)
        Logger.log("There is no cards in the EEPROM.");
    else {
        for (JsonObject card : doc["cards"].as<JsonArray>()) {
            const char* card_name = card["card_name"];
            JsonArray key = card["key"];
            int sector = card["sector"];
            cards.push_back(Card(card_name, key, sector));
        }
    }
}

std::vector<Card> Library::getCards() {
    return cards;
}

bool Library::add_card(Card* card) {
    //eeprom full check
    if (cards.size() >= 20) {
        Logger.log("You have reached the limit of learning 20 cards. To unlock this limit update the firmware.");
        return false;
    }
    if (write_key_on_card(card)) {
        cards.push_back(*card);
        if (update_cards_on_eeprom()) {
            Logger.log("Card learned successfully. Your card name is: " + card->card_name);
            return true;
        }
    }
    return false;
}

bool Library::reset_card() {
    if (rfid.PICC_IsNewCardPresent() and rfid.PICC_ReadCardSerial()) {
        if (check_card()) {
            Card card = get_card_by_scan();
            byte data_NormalKey[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            mifare_status = rfid.MIFARE_Write(sectors[card.sector], data_NormalKey, 16);
            if (mifare_status == MFRC522::STATUS_OK) {
                rfid.PICC_HaltA();
                rfid.PCD_StopCrypto1();
                for (uint8_t i = 0; i < cards.size(); i++)
                    if (cards[i] == card) {
                        cards.erase(cards.begin() + i);
                        break;
                    }
                update_cards_on_eeprom();
                Logger.log("The card has been reset successfully.");
                return true;
            }
            Logger.log("An error occurred while reseting the card.");
            return false;
        }
        else {
            Logger.log("Can not reset an unknown card.");
            return false;
        }
    }
    Logger.log("Card not detected for reseting.");
    return false;
}

Card Library::get_card_by_scan() {
    for (Card card : cards) {
        for (int i = 0; i < 6; i++) //key A
            mifare_key.keyByte[i] = card.key[i];
        mifare_status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, sectors[card.sector], &mifare_key, &(rfid.uid));
        if (mifare_status == MFRC522::STATUS_OK) {
            for (int i = 0; i < 6; i++) //key B
                mifare_key.keyByte[i] = card.key[i + 6];
            mifare_status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, sectors[card.sector], &mifare_key, &(rfid.uid));
            if (mifare_status == MFRC522::STATUS_OK) {
                return card;
            }
        }
        if (not rfid.PICC_IsNewCardPresent() or not rfid.PICC_ReadCardSerial())
            break;
    }
    return Card();
}

bool Library::write_key_on_card(Card* card) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        if (check_card()) {
            Logger.log("This card is learned before.");
            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
            return false;
        }
        //find a blank sector:
        int sector_number = 0;
        while (true) {
            for (int i = 0; i < 6; i++)
                mifare_key.keyByte[i] = 0xFF; // Default key in new cards
            mifare_status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, sectors[sector_number], &mifare_key, &(rfid.uid));
            if (mifare_status == MFRC522::STATUS_OK)
                break;
            sector_number++;
            if (sector_number == 16) {
                Logger.log("Can not find an empty sector.");
                return false;
            }
            if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
                Logger.log("Card lost at sector " + String(sector_number));
                return false;
            }
        }
        card->sector = sector_number;

        //insert the key in a buffer:
        byte buffer[16];
        const byte Access_Bits[] = { 0x7F, 0x07, 0x88, 0xAA };
        for (int i = 0; i < 6; i++)
            buffer[i] = card->key[i];
        for (int i = 0; i < 4; i++)
            buffer[i + 6] = Access_Bits[i];
        for (int i = 0; i < 6; i++)
            buffer[i + 10] = card->key[i + 6];

        //write on the card:
        mifare_status = rfid.MIFARE_Write(sectors[card->sector], buffer, 16);
        if (mifare_status == MFRC522::STATUS_OK) {
            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
            Logger.log("Writing on card was successfull.");
            return true;
        }
        else {
            Logger.log("Can not Write on card.");
            return false;
        }
    }
    Logger.log("Card not detected for learning.");
    return false;
}

bool Library::update_cards_on_eeprom() {
    StaticJsonDocument<3000> doc;
    JsonArray cardsArray = doc.createNestedArray("cards");
    for (Card card : cards) {
        JsonObject record = cardsArray.createNestedObject();
        record["card_name"] = card.card_name;
        record["sector"] = card.sector;
        JsonArray key = record.createNestedArray("key");
        for (int i = 0; i < 12; i++)
            key.add(card.key[i]);
    }
    char json_string[3000] = { '\0' };
    serializeJson(doc, json_string, sizeof(json_string));
    EEPROM.put(600, json_string);
    if (EEPROM.commit()) {
        return true;
    }
    Logger.log("An error occurred while updating the EEPROM.");
    return false;
}

bool Library::remove_card(int index) {
    cards.erase(cards.begin() + index);
    //Exception handling is disabled. So this exception cannot be handled. Be careful when using this function.
    //If you don't see next print, the application has aborted by an out of index error.
    Logger.log("Card removed successfully.");
    update_cards_on_eeprom();
    return true;
}

void Library::remove_all_cards() {
    cards.clear();
    update_cards_on_eeprom();
    Logger.log("All cards deleted successfully.");
}

bool Library::check_card() {
    for (Card card : cards) {
        for (int i = 0; i < 6; i++) //key A
            mifare_key.keyByte[i] = card.key[i];
        mifare_status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, sectors[card.sector], &mifare_key, &(rfid.uid));
        if (mifare_status == MFRC522::STATUS_OK) {
            for (int i = 0; i < 6; i++) //key B
                mifare_key.keyByte[i] = card.key[i + 6];
            mifare_status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, sectors[card.sector], &mifare_key, &(rfid.uid));
            if (mifare_status == MFRC522::STATUS_OK) {
                return true;
            }
        }
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
            break;
    }
    return false;
}

