#include "util.h"

enum LogLevel{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger{
    private:
    HardwareSerial* hserial;
    LogLevel filter;
    String levelToString(LogLevel level);

    public:
    Logger() = default;
    Logger(HardwareSerial* hserial, LogLevel filter);
    void log(const Printable &str, LogLevel level);
    void log(const String& str, LogLevel level);
    void log(const char* fmt, LogLevel level, ...);
};