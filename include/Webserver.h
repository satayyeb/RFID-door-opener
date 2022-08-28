#pragma once

#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include "Settings.h"
#include "hardwareConfig.h"
#include "Controller.h"
#include "Logger.h"



class Webserver {
private:
    Library* library;
    Controller* controller;
    Logger* logger;
public:
    ESP8266WiFiMulti wifiMulti;
    ESP8266WebServer server = ESP8266WebServer(80);
    Webserver(Settings* settings, Controller* controller, Library* library, Logger* logger);
    String get_file(String file_name);
    void configure_server_routings();
    void handelRequest();
};
