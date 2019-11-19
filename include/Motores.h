#ifndef MOTORES_H_
#define MOTORES_H_

#include <Arduino.h>

void MotoresSetup(void);

void MoverAdelante(void);
void MoverAtras(void);
void GirarDerecha(void);
void GirarIzquierda(void);
void Detener(void);

#endif