#include <Arduino.h>
#include <Accesspoint.h>
#include <Motores.h>
#include <FSM.h>
#include <Ultrasonico.h>
#include <Time.h>
#include <Cuello.h>
#include <Ticker.h>

ICACHE_RAM_ATTR void ISR_UpdateFSM(){
  FSM_UpdateState();
}
Ticker Timer_UpdateFSM(ISR_UpdateFSM, 100); 


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
  SERVO_Setup();
  FSM_Init();

  srand(time(NULL)); //Para invocar random ?????

  runMode = 0; //Modo Manual
  printed = 0;

}

void loop() {
  ToDoAction = AccessPoint_CheckClientPetition();
  if (runMode == 1 ){

    //Serial.println("Estamos en modo automático.");
    if (Timer_UpdateFSM.state() == 0){
      FSM_Init();
      Timer_UpdateFSM.start();
    }
    else
      Timer_UpdateFSM.update();
      
    FSM_DoState();

    //FSM_DoState();
    //FSM_UpdateState(); //Se deberia actualizar con timer
    /*if (!printed){
      Serial.println("Estamos en modo automático.");
      printed = 1;
    }*/
  }else{
    Timer_UpdateFSM.stop();
    FSM_Init();
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