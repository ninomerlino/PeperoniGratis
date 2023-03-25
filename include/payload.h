#include <Arduino.h>

class Payload {
    private:
    String serilized_data;

    public:
    Payload();
    template <typename T> void add_field(String key, T value);
    const char* to_string();
    void clear();
};
