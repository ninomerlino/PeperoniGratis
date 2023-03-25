#include "payload.h"
#include "logger.h"

extern Logger logger;

Payload::Payload(){
    this->serilized_data = "{";
}

template <typename T> void Payload::add_field(String key, T value){
    logger.log("Adding field: " + key + " with value: " + String(value), DEBUG);
    this->serilized_data += "\"" + key + "\":\"" + String(value) + "\",";
}

const char* Payload::to_string(){
    this->serilized_data.remove(this->serilized_data.length()-1);
    this->serilized_data += "}";
    logger.log("Payload: " + this->serilized_data, DEBUG);
    return this->serilized_data.c_str();
}

void Payload::clear(){
    logger.log("Clearing payload", DEBUG);
    this->serilized_data = "{";
}