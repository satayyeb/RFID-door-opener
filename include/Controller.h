#pragma once

#include "Arduino.h"
#include "GlobalObjects.h"
#include "hardwareConfig.h"

void open_door();
void reboot();
void factory_reset();
void dumpEEPROM(int size);
