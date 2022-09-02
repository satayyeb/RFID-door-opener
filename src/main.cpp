/*
	RFID DOOR OPENER (RDO) - version 3.0.0
	project  start date: Shahrivar 1398 , september 2019
	version3 start date: Mordad    1401 , August    2022
	author: Sayyed Ali Tayyeb
*/


// #include "LogContainer.h"
// #include "Settings.h"
// #include "Library.h"
// #include "Webserver.h"

#include "GlobalObjects.h"


void handel_card_scan();
void get_command_from_serial();

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

	webserver.init();


}

unsigned long timer;
unsigned long lockMillis;
int count_false_cards = 0;
bool RFID_lock = false;
bool last_reset_pin_state = LOW;
bool last_connection_status = true;


// void connect_to_wifi() {
// 	if (webserver.wifiMulti.run() != WL_CONNECTED) {
// 		if (last_connection_status == true) {
// 			last_connection_status = false;
// 			Serial.println("Looking for WiFi...");
// 		}
// 		Serial.print(".");
// 		delay(500);
// 	}
// 	else if (!last_connection_status) {
// 		last_connection_status = true;
// 		Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
// 	}
// }


wl_status_t last_wifi_stat = WL_DISCONNECTED;


void loop() {
	// connect_to_wifi();

	// handel http request:
	webserver.handelRequest();

	if (Serial.available()) 
		get_command_from_serial();
	

	if (!library.is_modifying) {
		// handel scaned card:
		if (library.rfid.PICC_IsNewCardPresent() and library.rfid.PICC_ReadCardSerial()) {
			if (RFID_lock)
				Logger.log("System is locked.");
			else
				handel_card_scan();
			library.rfid.PICC_HaltA();
			library.rfid.PCD_StopCrypto1();
		}
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
		Logger.log("Factory reset in progress...");
		factory_reset();
	}
}


void get_command_from_serial() {
	char command = Serial.read();
	if (command == 'm') {
		library.is_modifying = true;
		Serial.println("locked for modifying");
	}
	if (command == 'e') {
		library.is_modifying = false;
		Serial.println("unlocked");
	}
	else if (command == 'l') {
		Card card("test card");
		library.add_card(&card);
	}
	else if (command == 'r') {
		library.reset_card();
	}
	else if (command == 'd') {
		dumpEEPROM(2048);
	}
}

void handel_card_scan() {
	if (library.check_card()) {
		Logger.log("WELCOME");
		open_door();
		count_false_cards = 0;
	}
	else {
		Logger.log("card is unknown.");
		count_false_cards++;
		if (count_false_cards == 5) {
			Logger.log("several failed attempts. system is locked now.");
			RFID_lock = true;
			lockMillis = millis();
		}
	}
}


