
#include <Motores.h>

#define PinIN1 5
#define PinIN2 4
#define PinIN3 0  //RUEDA IZQ
#define PinIN4 2  //RUEDA IZQ
#define PinRelay 1

void MotoresSetup(){
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(PinRelay,OUTPUT);
  }

void MoverAdelante()
{
   digitalWrite (PinIN1, HIGH);
   digitalWrite (PinIN2, LOW);
   digitalWrite (PinIN3, HIGH);
   digitalWrite (PinIN4, LOW);
}

void MoverAtras(){
   digitalWrite (PinIN1, LOW);
   digitalWrite (PinIN2, HIGH);
   digitalWrite (PinIN3, LOW);
   digitalWrite (PinIN4, HIGH); 
}
/*
void GirarIzquierda(){
  digitalWrite (PinIN1, LOW);
  digitalWrite (PinIN2, LOW);
  digitalWrite (PinIN3, LOW);
  digitalWrite (PinIN4, HIGH);
}

void GirarDerecha(){
  digitalWrite (PinIN1, LOW);
  digitalWrite (PinIN2, HIGH);
  digitalWrite (PinIN3, LOW);
  digitalWrite (PinIN4, LOW); 
}
*/
 //GIRAR 2 RUEDAS
void GirarIzquierda(){
  digitalWrite (PinIN1, HIGH);
  digitalWrite (PinIN2, LOW);
  digitalWrite (PinIN3, LOW);
  digitalWrite (PinIN4, HIGH);
}
void GirarDerecha(){
  digitalWrite (PinIN1, LOW);
  digitalWrite (PinIN2, HIGH);
  digitalWrite (PinIN3, HIGH);
  digitalWrite (PinIN4, LOW); 
}

void Detener(){
  digitalWrite (PinIN1, LOW);
  digitalWrite (PinIN2, LOW);
  digitalWrite (PinIN3, LOW);
  digitalWrite (PinIN4, LOW);
}

void Aspiradora (){
  digitalWrite (PinRelay, HIGH); 
}

void NoAspiradora (){
  digitalWrite (PinRelay, LOW);
}