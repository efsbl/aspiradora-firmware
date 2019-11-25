#include <Ultrasonico.h>

#define trigPin  16
#define echoPin  14

// defines variables
long duration;
int distance;

void Ultrasonico_Setup(){
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

char Ultrasonico_Trigger(){
    char hayObstaculo = 0;
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

// Calculating the distance
  distance= duration*0.034/2;
  if (distance < 30)
  {
      hayObstaculo = 1;
  }
  
  return hayObstaculo;
}