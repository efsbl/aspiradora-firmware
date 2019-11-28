#include <Encoder.h>

#define pinEncoder 12
#define RANURAS 20
#define DIAMETRO 65

//ICACHE_RAM_ATTR

unsigned int cantPulses;


ICACHE_RAM_ATTR void ISR_Encoder(){
    cantPulses++;
}

void ENCODER_Start(){
    pinMode(pinEncoder, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinEncoder), ISR_Encoder, RISING);
}

void ENCODER_Stop(){
    detachInterrupt(pinEncoder);
}

void ENCODER_Reset(){
    cantPulses = 0;
}

int ENCODER_GetPulses(){
    return cantPulses;
}