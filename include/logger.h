#include "HardwareSerial.h"
#include "Printable.h"

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
    Logger(HardwareSerial* hserial, LogLevel filter=DEBUG);
    void log(const Printable &str, LogLevel level=DEBUG);
    void log(const String& str, LogLevel level=DEBUG);
    void log(const char* fmt, LogLevel level, ...);
};