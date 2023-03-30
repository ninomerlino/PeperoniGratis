#include <Arduino.h>

class Payload {
    private:
    String serilized_data;

    public:
    Payload();
    void addField(String key, float value);
    const char* toString();
    void clear();
};
