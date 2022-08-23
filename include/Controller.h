#pragma once

#include <queue>
#include "Arduino.h"
#include "hardwareConfig.h"
#include "Settings.h"
#include "Library.h"


class Controller {
private:
    Settings* settings;
    Library* library;
    std::queue<String> logs;
public:
    Controller();
    Controller(Settings* settings, Library* library);
    void open_door();
    void reboot();
    void factory_reset();
    void log(String log);
    String get_last_log();
    static void dumpEEPROM(int size);
};


