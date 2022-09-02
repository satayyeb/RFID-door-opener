#pragma once

#include <Arduino.h>
#include <queue>


class LogContainer {
private:
    std::queue<String> logs;
public:
    LogContainer();
    void log(String log);
    String get_last_log();
    String get_last_ten_logs();
};

// #if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_LOGGER)
// extern LogContainer Logger;
// #endif
