#pragma once

#include <queue>
#include "Arduino.h"
#include "hardwareConfig.h"
#include "Settings.h"
#include "Library.h"


static std::queue<String> logs;

class Controller {
private:
    Settings* settings;
    Library* library;
public:
    Controller();
    Controller(Settings* settings, Library* library);
    void open_door();
    void reboot();
    void factory_reset();
    static void log(String log);
    static String get_last_log();
    static void dumpEEPROM(int size);
};


