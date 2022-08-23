#pragma once

#include <Arduino.h>
#include <queue>


class Logger {
private:
    std::queue<String> logs;
public:
    Logger();
    ~Logger();
    void log(String log);
    String get_last_log();
    String get_last_ten_logs();
};
