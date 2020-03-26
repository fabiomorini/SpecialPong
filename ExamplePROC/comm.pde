/*
Aquest codi implementa la part de Processing d'un protocol de comunicació entre Arduino i Processing
En iniciar el codi, es demana el port al que està connectat Arduino (per exempoe: '3' -> COM3)
Existeixen tres canals de comunicació bidireccionals entre Arduino i Processing
Cada canal pot transportar un nombre de 4 xifres

Enviar dades Processing -> Arduino:

    if ( isConnected ) {
      sendData (channel, value), on:
      }

On, 
 - channel: CHANNEL_1, CHANNEL_2 o CHANNEL_3
 - value: número entre 0 i 9999

Rebre dades Arduino -> Processing:

    if ( isConnected ) {
      if ( dataAvailable(channel) ) {
        value = getData(channel)
      }
    }
    
On,
 - channel: CHANNEL_1, CHANNEL_2 o CHANNEL_3
 - value: número entre 0 i 9999

Veure exemple

Carles Vilella, 2017
*/

import processing.serial.*;

String arduinoCOM = "COM7";
boolean isConnected;
boolean comInitialized;

final int NUMBER_OF_DIGITS=4;  // Cannot only be changed if Arduino code is adapted
final int SP_BAUDRATE=115200;

// Serial port protocol

// Init communications Command
final String sp_GO = "GO";

final int CHANNEL_1 = 1;
final int CHANNEL_2 = 2;
final int CHANNEL_3 = 3;

// Pc to Arduino commands
final String sp_PCtoARD_CMD1 = "P1";
final String sp_PCtoARD_CMD2 = "P2";
final String sp_PCtoARD_CMD3 = "P3";
int PCtoARD_param1;
int PCtoARD_param2;
int PCtoARD_param3;
// PC -> ARDUINO: FLAGS
boolean PCtoARD_flag1;
boolean PCtoARD_flag2;
boolean PCtoARD_flag3;

// Arduino to PC commands
final String sp_ARDtoPC_CMD1 = "A1";
final String sp_ARDtoPC_CMD2 = "A2";
final String sp_ARDtoPC_CMD3 = "A3";
int ARDtoPC_param1;
int ARDtoPC_param2;
int ARDtoPC_param3;
boolean ARDtoPC_flag1;
boolean ARDtoPC_flag2;
boolean ARDtoPC_flag3;

Serial arduinoPort;
String arduinoCommand;
boolean haveArduinoCommand;
boolean textInput;

void commSetup () {

  haveArduinoCommand = false;
  
  PCtoARD_flag1 = false;
  PCtoARD_flag2 = false;
  PCtoARD_flag3 = false;
  ARDtoPC_flag1 = false;
  ARDtoPC_flag2 = false;
  ARDtoPC_flag3 = false;
  
  textInput = false;
  isConnected = false;
  comInitialized = false;

  System.out.println("Available serial ports: ");
  printArray(Serial.list());
  System.out.println("COM?");
}


boolean isPortConnected () {
  return (isConnected);
}
// *****
void commManager () {
  
  if ( ( isConnected == false ) && ( comInitialized == false ) ) {

    if ( textInput == true ) {    
      arduinoCOM = "COM"+str(key);      
      isConnected = false;
      arduinoPort = new Serial(this, arduinoCOM, SP_BAUDRATE); 
      arduinoPort.bufferUntil('\n');
      println("Trying to connect to serial port: "+arduinoCOM);
      comInitialized = true;
    }    
  } else if ( ( isConnected == false) && ( comInitialized == true ) ) {
    arduinoPort.write(sp_GO+'\n');    
  } else {     
    if ( PCtoARD_flag1 ) {      
      PCtoARD_flag1 = false;
      arduinoPort.write(sp_PCtoARD_CMD1+' '+nf(PCtoARD_param1, NUMBER_OF_DIGITS)+'\n');      
    } else {
    }

    if ( PCtoARD_flag2 ) {      
      PCtoARD_flag2 = false;
      arduinoPort.write(sp_PCtoARD_CMD2+' '+nf(PCtoARD_param2, NUMBER_OF_DIGITS)+'\n');      
    } else {
    }

    if ( PCtoARD_flag3 ) {      
      PCtoARD_flag3 = false;
      arduinoPort.write(sp_PCtoARD_CMD3+' '+nf(PCtoARD_param3, NUMBER_OF_DIGITS)+'\n');      
    } else {
    }    
  }
  
}

// *****
void keyPressed () {

  if ( ( key >= '0') && ( key <= '9') ) { 
    textInput = true;  
  }
  
}

// *****
void serialEvent (Serial p) {
  
  arduinoCommand = p.readString();
  
  if ( arduinoCommand.length() >= 2 ) {
    if ( arduinoCommand.substring(0, 2).trim().equals(sp_GO.trim()) ) {
      System.out.println("Go received");
      isConnected = true;   
    } else if ( arduinoCommand.substring(0, 2).trim().equals(sp_ARDtoPC_CMD1.trim()) ) {
      ARDtoPC_param1 = Integer.parseInt(arduinoCommand.substring(3).trim());
      ARDtoPC_flag1 = true;      
    } else if ( arduinoCommand.substring(0, 2).trim().equals(sp_ARDtoPC_CMD2.trim()) ) {
      ARDtoPC_param2 = Integer.parseInt(arduinoCommand.substring(3).trim());      
      ARDtoPC_flag2 = true;      
    } else if ( arduinoCommand.substring(0, 2).trim().equals(sp_ARDtoPC_CMD3.trim()) ) {
      ARDtoPC_param3 = Integer.parseInt(arduinoCommand.substring(3).trim());      
      ARDtoPC_flag3 = true;      
    } else {      
    }
  }
    
}

// *****
void sendData (int channel, int param) {

  if ( channel == CHANNEL_1 ) {
    PCtoARD_flag1 = true;
    PCtoARD_param1 = param;
  } else if ( channel == CHANNEL_2 ) {
    PCtoARD_flag2 = true;
    PCtoARD_param2 = param;
  } else if ( channel == CHANNEL_3 ) {
    PCtoARD_flag3 = true;
    PCtoARD_param3 = param;
  } else {
  }
  
}

// *****
boolean dataAvailable (int channel) {

  boolean available;

  available = false;
  if ( channel == CHANNEL_1 ) {
    available = ARDtoPC_flag1;
  } else if ( channel == CHANNEL_2 ) {
    available = ARDtoPC_flag2;
  } else if ( channel == CHANNEL_3 ) {
    available = ARDtoPC_flag3;
  } else {
  }

  return (available);
}

// *****
int getData (int channel) {

  int data;

  data = 0;
  if ( ( channel == CHANNEL_1 ) && ( ARDtoPC_flag1 == true ) ) {
    data = ARDtoPC_param1;
    ARDtoPC_flag1 = false;    
  } else if ( ( channel == CHANNEL_2 ) && ( ARDtoPC_flag2 == true ) ) {
    data = ARDtoPC_param2;
    ARDtoPC_flag2 = false;    
  } else if ( ( channel == CHANNEL_3 ) && ( ARDtoPC_flag3 == true ) ) {
   data = ARDtoPC_param3;
    ARDtoPC_flag3 = false;    
  } else {
  }

  return (data);
}
