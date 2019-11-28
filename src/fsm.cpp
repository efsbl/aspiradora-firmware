#include <FSM.h>
//https://github.com/sstaub/Ticker
State_Type currentState = OFF;
State_Type lastState;

char hayObstaculo;
char motorDone;
char servoDone;

void ISR_TimerServo(void);
char servoDoneRoutine;
Ticker Timer_Servo(ISR_TimerServo, 1500);
ICACHE_RAM_ATTR void ISR_TimerServo(){
    //hayObstaculo = Ultrasonico_Trigger();
    servoDoneRoutine = 1;
}


void ISR_TimerEncoder(void);
char encoderDone;
Ticker Timer_Encoder(ISR_TimerEncoder, 10);
ICACHE_RAM_ATTR void ISR_TimerEncoder(){
    int pulses = ENCODER_GetPulses();
    if (pulses > 36){
        encoderDone = 1;
        ENCODER_Stop();
    }
}

void FSM_Init(){
    hayObstaculo = 0;
    servoDone = 0;
    motorDone = 0;
    encoderDone = 0;
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
        delay(10);
        break; 

    case NEED_TO_AVOID:
        Detener();
        //delay(500);
        hayObstaculo = Ultrasonico_Trigger();
        delay(10);
        break;

    case AVOIDING:
        Detener();
        //delay(500);
        break;

    case LOOKING_RIGHT:
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
                delay(500);//delay???
                Timer_Encoder.stop();
                servoDone = 1;    
            }
        }
        /*
        SERVO_MirarDer();
        //delay(1000);
        hayObstaculo = Ultrasonico_Trigger();
        servoDone = 1;
        SERVO_MirarCentro();
        //delay(500);*/
        break;

    case LOOKING_LEFT:
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
                delay(500);
                servoDone = 1;    
            }
        }
        /*SERVO_MirarIzq();
        //delay(1000);
        hayObstaculo = Ultrasonico_Trigger();
        servoDone = 1;
        SERVO_MirarCentro();
        //delay(500);*/
        break;

    case TURNING_RIGHT:
        Serial.print("Estado: ");
        Serial.println(Timer_Encoder.state());
        if (Timer_Encoder.state() == 0){
            ENCODER_Reset();
            ENCODER_Start();
            GirarDerecha();
            Timer_Encoder.start();
        }else{
            if(!encoderDone && Timer_Encoder.state() == 1){
                Timer_Encoder.update();
            }
            else if(encoderDone){
                Timer_Encoder.stop();
                encoderDone = 0;
                Detener();
                delay(500);
                motorDone = 1;
            }
        }
        /*
        MoverAtras();
        //delay(500);
        Detener();
        //delay(500);
        GirarDerecha();
        //delay(1000);
        Detener();
        //delay(500);
        motorDone = 1;*/
        break;
    case TURNING_LEFT:
        if (Timer_Encoder.state() == 0){
            ENCODER_Reset();
            ENCODER_Start();
            GirarIzquierda();
            Timer_Encoder.start();
        }else{
            if (!encoderDone && Timer_Encoder.state() == 1)
                Timer_Encoder.update();
            else if(encoderDone){
                Timer_Encoder.stop();
                encoderDone = 0;
                Detener();
                delay(500);
                motorDone = 1;
            }
        }
        /*MoverAtras();
        //delay(500);
        Detener();
        //delay(500);
        GirarIzquierda();
        //delay(1000);
        Detener();
        //delay(500);
        motorDone = 1;*/
        break;

    case TURNING_AROUND:
        /*MoverAtras();
        //delay(500);
        Detener();
        //delay(500);
        GirarIzquierda();
        //delay(1500);
        Detener();
        //delay(500);*/
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