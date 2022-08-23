#include "Card.h"

Card::Card() {

}

Card::Card(String card_name) {
    this->card_name = card_name;
    //generate a new key
    for (int i = 0; i < 12; i++)
        key[i] = random(254); // make a random number from 0 to 253
}

Card::Card(const char* card_name, JsonArray key, int sector) {
    this->card_name = String(card_name);
    this->sector = sector;
    for (int i = 0; i < 12; i++)
        this->key[i] = key[i];
}

bool Card::operator == (const Card& obj) const {
    if (card_name != obj.card_name)
        return false;
    for (int i = 0; i < 12; i++)
        if (key[i] != obj.key[i])
            return false;
    return true;
}