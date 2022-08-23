#include "Logger.h"


Logger::Logger() {}


Logger::~Logger() {}


void Logger::log(String log) {
    Serial.println(log);
    logs.push(log);
    while (logs.size() >= 20)
        logs.pop();
}


String Logger::get_last_log() {
    if (logs.empty())
        return "No log";
    return logs.back();
}


String Logger::get_last_ten_logs() {
    //TODO: implement this...
    return "No implement.";
}