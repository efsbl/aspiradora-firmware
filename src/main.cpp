#include <Arduino.h>
#include <Accesspoint.h>
#include <Motores.h>

Action_Type ToDoAction;

void setup() {

  Serial.begin(115200);
  while(!Serial);

  AccessPoint_Setup();

  MotoresSetup();

}

void loop() {

  ToDoAction = AccessPoint_CheckClientPetition();
  switch (ToDoAction)
  {
  case AVANZAR:
    MoverAdelante();
    break;
  case RETROCEDER:
    MoverAtras();
    break;
  case GIRARIZQ:
    GirarIzquierda();
    break;
  case GIRARDER:
    GirarDerecha();
    break;
  case DETENER:
    Detener();
    break; 
  default: //INIT
    break;
  }
  AccessPoint_Flush();

}