#include "Webserver.h"
#include "GlobalObjects.h"


void Webserver::init() {
    // ESP.eraseConfig();
    WiFi.mode(WIFI_AP);
    // WiFi.setAutoReconnect(true);
    // WiFi.persistent(true);
    // WiFi.begin("tel", "hajiRouterHamRouterHayeGhadim");

    // Serial.print("Connecting to WiFi ..");
    // while (WiFi.status() != WL_CONNECTED) {
    //     Serial.print('.');
    //     delay(1000);
    // }

    WiFi.softAPConfig(settings.ip, settings.ip, settings.subnet);
    WiFi.softAP(settings.wifi_ssid, settings.wifi_pass);
    Logger.log("Access Point started at: " + WiFi.softAPIP().toString());
    configure_server_routings();
    server.begin();

    if (LittleFS.begin())
        Logger.log("Filesystem started successfully.");
    else
        Logger.log("Filesystem starting failed.");
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


void Webserver::handelRequest() {
    server.handleClient();
}

String get_file_extension(String file_name) {
    int index = file_name.lastIndexOf(".");
    return file_name.substring(index + 1);
}

void Webserver::configure_server_routings() {
    server.on("/", [&]() {
        server.send(200, "text/html", get_file("index.html"));
        library.is_modifying = false;
        });
    server.on("/RFID.html", [&]() {
        server.send(200, "text/html", get_file("RFID.html"));
        library.is_modifying = true;
        });
    server.on("/system.html", [&]() {
        server.send(200, "text/html", get_file("system.html"));
        library.is_modifying = false;
        });
    server.onNotFound([&]() {
        String file_content = get_file(server.uri());
        String file_type = get_file_extension(server.uri());
        if (file_type == "html")
            server.send(200, "text/html", file_content);
        else if (file_type == "css")
            server.send(200, "text/css", file_content);
        else if (file_type == "js")
            server.send(200, "application/javascript", file_content);
        else if (file_type == "ico")  //TODO: add favicon.ico
            server.send(404, "text/html", "favicon not found");
        else
            server.send(200, "text/html", file_content);
        });
    ///commands:
    server.on("/open", [&]() {
        server.sendHeader("Location", "/");
        server.send(301, "text/html", "The door open successfully.");
        open_door();
        });
    server.on("/reboot", [&]() {
        server.sendHeader("Location", "/");
        server.send(301, "text/html", "System reboot is in progress...");
        // delay(500);
        reboot();
        });
    server.on("/learn", [&]() {
        int card_number = library.getCards().size() + 1;
        Card card("card " + String(card_number));
        library.add_card(&card);
        server.send(200, "text/html", Logger.get_last_log());
        });
    server.on("/reset", [&]() {
        library.reset_card();
        server.send(200, "text/html", Logger.get_last_log());
        });
    server.on("/delete-all", [&]() {
        library.remove_all_cards();
        server.send(200, "text/html", Logger.get_last_log());
        });
    server.on("/get-cards", [&]() {
        String html = "<h1>Cards List</h1>\n<ul style='text-align: left;'>";
        for (Card card : library.getCards())
            html += "<li>" + card.card_name + "</li>\n";
        html + "</ul>";
        server.send(200, "text/html", html);
        });
    server.on("/get-cards-in-json", [&]() {
        //TODO: this is static memory size and may has errors fix it later.
        StaticJsonDocument<1000> doc;
        JsonArray cards = doc.createNestedArray("cards");
        for (Card card : library.getCards())
            cards.add(card.card_name);
        String json;
        serializeJson(doc, json);
        server.send(200, "text/json", json);
        });
}

