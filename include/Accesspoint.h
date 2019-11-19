#ifndef ACCESSPOINT_H_
#define ACCESSPOINT_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>

enum Action_Type {
    INIT,
    AVANZAR,
    RETROCEDER,
    DETENER,
    GIRARIZQ,
    GIRARDER,
};

void AccessPoint_Setup(void);
Action_Type AccessPoint_CheckClientPetition(void);
void AccessPoint_Flush(void);

#endif