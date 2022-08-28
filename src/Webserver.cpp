#include "Webserver.h"


Webserver::Webserver(Settings* settings, Controller* controller, Library* library, Logger* logger) {
    this->controller = controller;
    this->library = library;
    this->logger = logger;
    LittleFS.begin();


    // WiFi.mode(WIFI_AP_STA);

    // WiFi.softAPConfig(settings->ip, settings->ip, settings->subnet);
    // WiFi.softAP("ali", settings->wifi_pass, 11);
    // logger->log("Access Point started at: " + WiFi.softAPIP().toString());
    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_AP);
    // WiFi.begin("tel", "hajiRouterHamRouterHayeGhadim");
    // WiFi.begin("shatelmobile8242", "35108242");
    // WiFi.mode(WIFI_STA);
    // WiFi.begin("desktop", "asdfghjk");




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

    server.on("/backup.html", [&]()
        { server.send(200, "text/html", get_file("backup.html")); });
    server.on("/footer.html", [&]()
        { server.send(200, "text/html", get_file("footer.html")); });
    server.on("/form.html", [&]()
        { server.send(200, "text/html", get_file("form.html")); });
    server.on("/header.html", [&]()
        { server.send(200, "text/html", get_file("header.html")); });
    server.on("/import.html", [&]()
        { server.send(200, "text/html", get_file("import.html")); });
    server.on("/", [&]() {
        server.send(200, "text/html", get_file("index.html"));
        library->is_modifying = false;
        });
    server.on("/login-settings.html", [&]()
        { server.send(200, "text/html", get_file("login-settings.html")); });
    server.on("/RFID.html", [&]() {
        server.send(200, "text/html", get_file("RFID.html"));
        library->is_modifying = true;
        });
    server.on("/select-card-form.html", [&]()
        { server.send(200, "text/html", get_file("select-card-form.html")); });
    server.on("/system.html", [&]() {
        server.send(200, "text/html", get_file("system.html"));
        library->is_modifying = false;
        });
    server.on("/update-frameware.html", [&]()
        { server.send(200, "text/html", get_file("update-frameware.html")); });
    server.on("/wifi-settings.html", [&]()
        { server.send(200, "text/html", get_file("wifi-settings.html")); });


    // server.on("/favicon.ico", [&]() {
    //     server.send(200, "image/png", get_file("favicon.ico"));
    //     });

    server.on("/style.css", [&]()
        { server.send(200, "text/css", get_file("style.css")); });


    server.on("/js/home-script.js", [&]()
        { server.send(200, "application/javascript", get_file("js/home-script.js")); });
    server.on("/js/loader.js", [&]()
        { server.send(200, "application/javascript", get_file("js/loader.js")); });
    server.on("/js/rfid-script.js", [&]()
        { server.send(200, "application/javascript", get_file("js/rfid-script.js")); });
    server.on("/js/system-script.js", [&]()
        { server.send(200, "application/javascript", get_file("js/system-script.js")); });

    server.on("/open", [&]() {
        server.sendHeader("Location", "/");
        server.send(301, "text/html", "The door open successfully.");
        controller->open_door();
        });
    server.on("/reboot", [&]() {
        server.sendHeader("Location", "/");
        server.send(301, "text/html", "System reboot is in progress...");
        // delay(500);
        controller->reboot();
        });
    server.on("/learn", [&]() {
        Card card("card");
        library->add_card(&card);
        server.send(200, "text/html", logger->get_last_log());
        });
    server.on("/reset", [&]() {
        library->reset_card();
        server.send(200, "text/html", logger->get_last_log());
        });
    server.on("/delete-all", [&]() {
        library->remove_all_cards();
        server.send(200, "text/html", logger->get_last_log());
        });
    server.on("/get-cards", [&]() {
        String html = "<h1>Cards List</h1>\n<ul style='text-align: left;'>";
        for (Card card : library->getCards())
            html += "<li>" + card.card_name + "</li>\n";
        html + "</ul>";
        server.send(200, "text/html", html);
        });



}

void Webserver::handelRequest() {
    server.handleClient();
}
