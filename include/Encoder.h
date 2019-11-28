
#ifndef ENCODER_H_
#define ENCODER_H_

#include <Arduino.h>

void ENCODER_Start(void);
void ENCODER_Stop(void);
void ENCODER_Reset(void);
int ENCODER_GetPulses(void);

#endif