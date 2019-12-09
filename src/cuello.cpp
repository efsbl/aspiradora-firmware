#include <Cuello.h>

#define pinServo 13

Servo myServo;

void SERVO_Setup(){
    myServo.attach(pinServo);
    myServo.write(90);
}

void SERVO_MirarDer(){
    myServo.write(10);
}

void SERVO_MirarIzq(){
    myServo.write(180);
}

void SERVO_MirarCentro(){
    myServo.write(90);
}


