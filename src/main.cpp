/*
	RFID DOOR OPENER (RDO) - version 3.0.0
	project  start date: Shahrivar 1398 , september 2019
	version3 start date: Mordad    1401 , August    2022
	author: Sayyed Ali Tayyeb
*/

#include "controller.h"
#include "Webserver.h"
#include "Settings.h"
#include "Library.h"
#include "Logger.h"
#include "Card.h"


void handel_card_scan();


Logger logger;
Settings settings(&logger);
Library library(&logger);
Controller controller(&settings, &library, &logger);
Webserver webserver(&settings, &controller, &library, &logger);


void setup() {
	pinMode(DOOR_RELAY, OUTPUT);
	digitalWrite(DOOR_RELAY, LOW);

	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_RED, HIGH);   // because my RGB LED is common Anode
	digitalWrite(LED_GREEN, HIGH); // because my RGB LED is common Anode

	pinMode(RESET_PIN, INPUT);
	pinMode(RANDOM_PIN, INPUT);
	randomSeed(analogRead(RANDOM_PIN));
}


unsigned long timer;
unsigned long lockMillis;
int count_false_cards = 0;
bool RFID_lock = false;
bool last_reset_pin_state = LOW;

void loop() {
	// handel http request:
	webserver.handelRequest();

	// handel scaned card:
	if (library.rfid.PICC_IsNewCardPresent() and library.rfid.PICC_ReadCardSerial()) {
		if (RFID_lock)
			logger.log("System is locked.");
		else
			handel_card_scan();
		library.rfid.PICC_HaltA();
		library.rfid.PCD_StopCrypto1();
	}

	// Locking timer of RFID brute force attack:
	if (RFID_lock and (millis() - lockMillis > 2 * 60 * 1000)) {
		RFID_lock = false;
		count_false_cards = 0;
	}

	// handel reset button:
	bool reset_pin_state = digitalRead(RESET_PIN);
	if (reset_pin_state != last_reset_pin_state) {
		last_reset_pin_state = reset_pin_state;
		timer = millis();
	}
	if (reset_pin_state == HIGH and (millis() - timer > 10 * 1000)) {
		logger.log("Factory reset in progress...");
		controller.factory_reset();
	}
}


void handel_card_scan() {
	if (library.check_card()) {
		logger.log("WELCOME");
		controller.open_door();
		count_false_cards = 0;
	}
	else {
		logger.log("card is unknown.");
		count_false_cards++;
		if (count_false_cards == 3) {
			logger.log("several failed attempts. system is locked now.");
			RFID_lock = true;
			lockMillis = millis();
		}
	}
}


