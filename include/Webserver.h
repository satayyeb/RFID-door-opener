#pragma once

#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>


class Webserver {
private:
public:
    ESP8266WebServer server = ESP8266WebServer(80);
    void init();
    String get_file(String file_name);
    void handelRequest();
    void configure_server_routings();
};

