#include "logger.h"

Logger::Logger(HardwareSerial* hserial, LogLevel filter=DEBUG){
    this->hserial = hserial;
    this->filter = filter;
}

void Logger::log(const Printable &str, LogLevel level=DEBUG){
    if(level >= filter){
        hserial->print(levelToString(level));
        hserial->print(": ");
        hserial->println(str);
    }
}

void Logger::log(const String &str, LogLevel level=DEBUG){
    if(level >= filter){
        hserial->print(levelToString(level));
        hserial->print(": ");
        hserial->println(str);
    }
}

void Logger::log(const char* fmt, LogLevel level, ...){
    if(level >= filter){
        hserial->print(levelToString(level));
        hserial->print(": ");
        va_list args;
        va_start(args, fmt);
        hserial->printf(fmt, args);
        va_end(args);
        hserial->println();
    }
}