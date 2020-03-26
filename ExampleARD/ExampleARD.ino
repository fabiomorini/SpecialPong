#include "comm.h"

//Potenciometro
const int analogPin = A0;    //Pin del Potenciometro
int value;      //variable que almacena la lectura analógica raw
int position;   //posicion del potenciometro en tanto por ciento

void setup()
{
  commSetup();
  Serial.begin(115200);
}

void loop()
{
  //Codigo del Potenciometro
  value = analogRead(analogPin);          // realizar la lectura analógica raw
  position = map(value, 0, 1023, 0, 100);  // convertir a porcentaje
  Serial.println(position);
  if ( portIsConnected ) 
  {
      sendData (CHANNEL_2, position);
  }
  commManager();    
}
