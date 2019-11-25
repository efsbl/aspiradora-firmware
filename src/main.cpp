#include <Arduino.h>
#include <Accesspoint.h>
#include <Motores.h>
#include <FSM.h>
#include <Ultrasonico.h>

void updateManual(void);

Action_Type ToDoAction;

unsigned char runMode;
unsigned char printed;

void setup() {

  Serial.begin(115200);
  while(!Serial);

  AccessPoint_Setup();
  MotoresSetup();
  Ultrasonico_Setup();

  runMode = 0; //Modo Manual
  printed = 0;

}

void loop() {
  
  ToDoAction = AccessPoint_CheckClientPetition();
  if (runMode == 1 ){
    FSM_DoState();
    FSM_UpdateState();
    Serial.println("Estamos en modo automático.");
    /*if (!printed){
      Serial.println("Estamos en modo automático.");
      printed = 1;
    }*/
  }else{
    if (printed){
      printed = 0;
    }
    updateManual();
  }

}

void updateManual(){
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