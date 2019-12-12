#include <FSM.h>
#include <Accesspoint.h>
#define ENCODER_INTERRUPTS_90 24
#define ENCODER_INTERRUPTS_180 45
//https://github.com/sstaub/Ticker

String estado = "";

State_Type currentState = OFF;
State_Type lastState;

char hayObstaculo;
char motorDone;
char servoDone;


void ISR_TimerServo(void);
char servoDoneRoutine;
Ticker Timer_Servo(ISR_TimerServo, 1500);
ICACHE_RAM_ATTR void ISR_TimerServo(){
    servoDoneRoutine = 1;
}


void ISR_TimerEncoder(void);
char encoderDone;
int cantidadInterrupciones;
Ticker Timer_Encoder(ISR_TimerEncoder, 10);
ICACHE_RAM_ATTR void ISR_TimerEncoder(){
    cantidadInterrupciones = ENCODER_INTERRUPTS_90;;
    if (currentState == TURNING_AROUND)
        cantidadInterrupciones = ENCODER_INTERRUPTS_180;
    //int pulses = ENCODER_GetPulses();
    if (ENCODER_GetPulses() > cantidadInterrupciones){
        encoderDone = 1;
        //ENCODER_Stop();
    }
}

void FSM_Init(){
    Detener();
    hayObstaculo = 0;
    servoDone = 0;
    motorDone = 0;
    encoderDone = 0;
    currentState = OFF;
    lastState = OFF;
    
    NoAspiradora();
}


void FSM_DoState(){
    switch (currentState)
    {
    case OFF:
        estado = "OFF";
        Detener();
        Aspiradora();
        break;
        

    case MOVING:
        estado = "MOVING";
        MoverAdelante();
        hayObstaculo = Ultrasonico_Trigger();
        delay(10);
        break; 

    case NEED_TO_AVOID:
        estado = "NEED_TO_AVOID";
        Detener();
        delay(200);
        hayObstaculo = Ultrasonico_Trigger();
        delay(10);
        break;

    case AVOIDING:
        estado = "AVOIDING";
        Detener();
        //delay(500);
        break;

    case LOOKING_RIGHT:
        estado = "LOOKING_RIGHT";
        if (Timer_Servo.state() == 0){
            SERVO_MirarDer();
            servoDoneRoutine = 0;
            Timer_Servo.start();
        }else{
            if (!servoDoneRoutine && Timer_Servo.state() == 1){
                Timer_Servo.update();
            }else if(servoDoneRoutine){
                Timer_Servo.stop();
                servoDoneRoutine = 0;
                hayObstaculo = Ultrasonico_Trigger();
                delay(10);
                SERVO_MirarCentro();
                delay(200);
                servoDone = 1;    
            }
        }
        break;

    case LOOKING_LEFT:
        estado = "LOOKING_LEFT";
        if (Timer_Servo.state() == 0){
            SERVO_MirarIzq();
            servoDoneRoutine = 0;
            Timer_Servo.start();    
        }else{
            if (!servoDoneRoutine && Timer_Servo.state() == 1){
                Timer_Servo.update();
            }else if (servoDoneRoutine){
                Timer_Servo.stop();
                servoDoneRoutine = 0;
                hayObstaculo = Ultrasonico_Trigger();
                delay(10);
                SERVO_MirarCentro();
                delay(200);
                servoDone = 1;    
            }
        }
        break;

    case TURNING_RIGHT:
        estado = "TURNING_RIGHT";
        if (Timer_Encoder.state() == 0){
            ENCODER_Reset();
            ENCODER_Start();
            delay(10);
            GirarDerecha();
            Timer_Encoder.start();
        }else{
            if(!encoderDone && Timer_Encoder.state() == 1){
                Timer_Encoder.update();
            }
            else if(encoderDone){
                Timer_Encoder.stop();
                ENCODER_Stop();
                encoderDone = 0;
                Detener();
                delay(200);
                motorDone = 1;
            }
        }
        break;
    case TURNING_LEFT:
        estado = "TURNING_LEFT";
        if (Timer_Encoder.state() == 0){
            ENCODER_Reset();
            ENCODER_Start();
            delay(10);
            GirarIzquierda();
            Timer_Encoder.start();
        }else{
            if (!encoderDone && Timer_Encoder.state() == 1)
                Timer_Encoder.update();
            else if(encoderDone){
                Timer_Encoder.stop();
                ENCODER_Stop();
                encoderDone = 0;
                Detener();
                delay(200);
                motorDone = 1;
            }
        }
        break;

    case TURNING_AROUND:
        estado = "TURNING_AROUND";
        if (Timer_Encoder.state() == 0){
            ENCODER_Reset();
            ENCODER_Start();
            delay(10);
            GirarIzquierda();
            Timer_Encoder.start();
        }else{
            if(!encoderDone && Timer_Encoder.state() == 1){
                Timer_Encoder.update();
            }
            else if(encoderDone){
                Timer_Encoder.stop();
                ENCODER_Stop();
                encoderDone = 0;
                Detener();
                delay(200);
                motorDone = 1;
            }
        }
        break;

    default:
        break;
    }
}

void FSM_UpdateState(){
    //AccessPoint_Flush();
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