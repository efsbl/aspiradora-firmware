#include <FSM.h>

State_Type currentState = OFF;
State_Type lastState;

char hayObstaculo;

void FSM_DoState(){
    switch (currentState)
    {
    case OFF:
        Detener();
        break;

    case MOVING:
        MoverAdelante();
        hayObstaculo = Ultrasonico_Trigger();
        break; 

    case NEED_TO_AVOID:
        Detener();
        hayObstaculo = Ultrasonico_Trigger();
        break;

    case AVOIDING:
        Detener();
        MoverAtras();
        delay(1000);
        Detener();
        GirarDerecha();
        delay(500);
        Detener();
        break;

    default:
        break;
    }
}

void FSM_UpdateState(){
    switch (currentState)
    {
    case OFF:
        if (runMode == 1){
            lastState = currentState;
            currentState = MOVING;
        }
        break;
    
    case MOVING:
        if (hayObstaculo) // && lastState = TURNING ARROUND
        lastState = currentState;
        currentState = NEED_TO_AVOID;
        break;
    
    case NEED_TO_AVOID:
        lastState = currentState;
        if (hayObstaculo)
            currentState = AVOIDING;
        else
            currentState = MOVING;
        break;
    
    case AVOIDING:
        lastState = currentState;
        currentState = MOVING;
        break;

    default:
        break;
    }
}