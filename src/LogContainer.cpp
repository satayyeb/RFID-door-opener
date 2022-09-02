#include "LogContainer.h"
#include "GlobalObjects.h"


LogContainer::LogContainer() {
    Serial.begin(115200);
}

void LogContainer::log(String log) {
    Serial.println(log);
    logs.push(log);
    while (logs.size() >= 20)
        logs.pop();
}


String LogContainer::get_last_log() {
    if (logs.empty())
        return "No log";
    return logs.back();
}


String LogContainer::get_last_ten_logs() {
    //TODO: implement this...
    return "No implement.";
}


// #if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_LOGGER)
// LogContainer Logger;
// #endif