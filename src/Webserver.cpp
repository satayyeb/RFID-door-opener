#include "Webserver.h"


Webserver::Webserver(Settings* settings, Controller* controller, Library* library) {
    this->controller = controller;
    this->library = library;
    LittleFS.begin();
    WiFi.softAPConfig(settings->ip, settings->ip, settings->subnet);
    WiFi.softAP(settings->wifi_ssid, settings->wifi_pass);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Access Point started at: ");
    Serial.println(myIP);
    configure_server_routings();
    server.begin();
}

String Webserver::get_file(String file_name) {
    File file = LittleFS.open("/" + file_name, "r");
    if (!file) {
        server.send(404, "text/html", "Page not found");
        return "not found";
    }
    String content;
    while (file.available())
        content += file.readString();
    file.close();
    return content.c_str();
}

void Webserver::configure_server_routings() {
    server.on("/", [&]()
        { server.send(200, "text/html", get_file("index.html")); });
    server.on("/learn.html", [&]()
        { server.send(200, "text/html", get_file("learn.html")); });
    server.on("/footer.html", [&]()
        { server.send(200, "text/html", get_file("footer.html")); });
    server.on("/header.html", [&]()
        { server.send(200, "text/html", get_file("header.html")); });
    server.on("/form.html", [&]()
        { server.send(200, "text/html", get_file("form.html")); });
    server.on("/select-card-form.html", [&]()
        { server.send(200, "text/html", get_file("select-card-form.html")); });
    server.on("/style.css", [&]()
        { server.send(200, "text/css", get_file("style.css")); });
    server.on("/script.js", [&]()
        { server.send(200, "application/javascript", get_file("script.js")); });
    server.on("/loader.js", [&]()
        { server.send(200, "application/javascript", get_file("loader.js")); });

    server.on("/open", [&]()
        { server.send(200, "text/html", get_file("index.html"));
    controller->open_door(); });
    server.on("/reboot", [&]()
        { server.send(200, "text/html", "rebooting...");
    controller->reboot(); });

    server.on("/learn", [&]() {
        Card card("card");
        library.add_card(card);
        server.send(200, "text/html", "rebooting...");
        });
}

void Webserver::handelRequest() {
    server.handleClient();
}
