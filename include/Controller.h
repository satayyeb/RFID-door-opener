#pragma once

#include "Arduino.h"
#include "hardwareConfig.h"
#include "Settings.h"
#include "Library.h"
#include "Logger.h"


class Controller {
private:
    Logger* logger;
    Settings* settings;
    Library* library;
public:
    Controller(Settings* settings, Library* library, Logger* logger);
    void open_door();
    void reboot();
    void factory_reset();
    static void dumpEEPROM(int size);
};


