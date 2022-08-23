#pragma once

#include "Arduino.h"
#include "hardwareConfig.h"
#include "Settings.h"
#include "Library.h"


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
    static void dumpEEPROM(int size);
};


