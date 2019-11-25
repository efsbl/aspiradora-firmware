#ifndef FSM_H_
#define FSM_H_

#include <Arduino.h>
#include <Motores.h>
#include <Ultrasonico.h>
#include <Time.h>
#include <Cuello.h>

void FSM_Init(void);
void FSM_DoState(void);
void FSM_UpdateState(void);

extern unsigned char runMode;

enum State_Type {
    OFF, 
    MOVING, 
    LOOKING_LEFT,
    LOOKING_RIGHT,
    TURNING_LEFT,
    TURNING_RIGHT,
    NEED_TO_AVOID,
    AVOIDING,
    TURNING_AROUND
};

#endif