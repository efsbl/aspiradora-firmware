#include <FSM.h>

State_Type currentState = OFF;
State_Type lastState;

char hayObstaculo;
char servoDone;
char motorDone;

void FSM_Init(){
    hayObstaculo = 0;
    servoDone = 0;
    motorDone = 0;
    currentState = OFF;
    lastState = OFF;
}

void FSM_DoState(){
    switch (currentState)
    {
    case OFF:
        Detener();
        break;

    case MOVING:
        MoverAdelante();
        hayObstaculo = Ultrasonico_Trigger();
        delay(1);
        break; 

    case NEED_TO_AVOID:
        Detener();
        delay(500);
        hayObstaculo = Ultrasonico_Trigger();
        break;

    case AVOIDING:
        Detener();
        delay(500);
        break;

    case LOOKING_RIGHT:
        SERVO_MirarDer();
        delay(1000);
        hayObstaculo = Ultrasonico_Trigger();
        servoDone = 1;
        SERVO_MirarCentro();
        delay(500);
        break;

    case LOOKING_LEFT:
        SERVO_MirarIzq();
        delay(1000);
        hayObstaculo = Ultrasonico_Trigger();
        servoDone = 1;
        SERVO_MirarCentro();
        delay(500);
        break;

    case TURNING_RIGHT:
        MoverAtras();
        delay(500);
        Detener();
        delay(500);
        GirarDerecha();
        delay(1000);
        Detener();
        delay(500);
        motorDone = 1;
        break;

    case TURNING_LEFT:
        MoverAtras();
        delay(500);
        Detener();
        delay(500);
        GirarIzquierda();
        delay(1000);
        Detener();
        delay(500);
        motorDone = 1;
        break;

    case TURNING_AROUND:
        MoverAtras();
        delay(500);
        Detener();
        delay(500);
        GirarIzquierda();
        delay(1500);
        Detener();
        delay(500);
        motorDone = 1;
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
        if (hayObstaculo){
            lastState = currentState;
            currentState = NEED_TO_AVOID;
        }
        break;
    
    case NEED_TO_AVOID:
        lastState = currentState;
        if (hayObstaculo)
            currentState = AVOIDING;
        else
            currentState = MOVING;
        break;
    
    case AVOIDING:
        if (rand() % 2 == 0){ //Es par
            lastState = currentState;
            currentState = LOOKING_RIGHT;
        }else{
            lastState = currentState;
            currentState = LOOKING_LEFT;
        }
        break;

    case LOOKING_RIGHT:
        if (servoDone){
            servoDone = 0; //False
            if (!hayObstaculo){
                lastState = currentState;
                currentState = TURNING_RIGHT;
            }else{
                if (lastState != LOOKING_LEFT){
                    lastState = currentState;
                    currentState = LOOKING_LEFT;
                }else{
                    lastState = currentState;
                    currentState = TURNING_AROUND;
                }
            }            
        }
        break;
    
    case LOOKING_LEFT:
        if (servoDone){
            servoDone = 0; //False
            if (!hayObstaculo){
                lastState = currentState;
                currentState = TURNING_LEFT;
            }else{
                if (lastState != LOOKING_RIGHT){
                    lastState = currentState;
                    currentState = LOOKING_RIGHT;
                }else{
                    lastState = currentState;
                    currentState = TURNING_AROUND;
                }
            }            
        }
        break;
    
    case TURNING_LEFT:
    case TURNING_RIGHT:
    case TURNING_AROUND:
        if (motorDone){
            motorDone = 0;
            lastState = currentState;
            currentState = MOVING;
        }
        break;

    default:
        break;
    }
}