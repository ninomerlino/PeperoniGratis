#include "payload.h"

Payload::Payload(){
    this->serilized_data = "{";
}

void Payload::addField(String key, float value){
    Serial.println("[PAYL] Adding field: " + key + " with value: " + String(value));
    this->serilized_data += "\"" + key + "\":\"" + String(value) + "\",";
}

const char* Payload::toString(){
    this->serilized_data.remove(this->serilized_data.length()-1);
    this->serilized_data += "}";
    Serial.println("[PAY] Payload: " + this->serilized_data);
    return this->serilized_data.c_str();
}

void Payload::clear(){
    Serial.println("[PAYL] Clearing payload");
    this->serilized_data = "{";
}