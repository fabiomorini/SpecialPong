/*
Aquest codi implementa la part d'Arduino d'un protocol de comunicació entre Arduino i Processing
Existeixen tres canals de comunicació bidireccionals entre Arduino i Processing.
Cada canal pot transportar un nombre de 4 xifres

Enviar dades Arduino -> Processsing:

    if ( isConnected ) {
      sendData (channel, value), on:
      }

On, 
 - channel: CHANNEL_1, CHANNEL_2 o CHANNEL_3
 - value: número entre 0 i 9999

Rebre dades Processing -> Arduino:

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

#include "comm.h"

// Serial port params
#define SP_BAUDRATE 115200
#define MAX_COMMAND 25    // Maximum length of a command (including TAG + value)
#define COMMAND_LENGTH 2  // Lenght of the TAG

// Variables to receive commands from the PC
char pcCommand[MAX_COMMAND];
int indexCommand;
boolean haveCommand;

// Text buffer to send command to the PC
char tmp_buf[MAX_COMMAND];

// Serial port protocol

// Init communications Command
char sp_GO[] = {'G', 'O', '\0'};       // Power up COMM

// PC -> Arduino: TAGS
char sp_PCtoARD_CMD1[] = {'P', '1', '\0'};      // PC to Arduino Command 1 TAG
char sp_PCtoARD_CMD2[] = {'P', '2', '\0'};      // PC to Arduino Command 2 TAG
char sp_PCtoARD_CMD3[] = {'P', '3', '\0'};      // PC to Arduino Command 3 TAG
// PC -> Arduino: PARAMS
int PCtoARD_param1;
int PCtoARD_param2;
int PCtoARD_param3;
// PC -> Arduino: FLAGS
boolean PCtoARD_flag1;
boolean PCtoARD_flag2;
boolean PCtoARD_flag3;

// Arduino -> PC: TAGS
char sp_ARDtoPC_CMD1[] =   {'A', '1'};  // Arduino to PC Command 1 TAG
char sp_ARDtoPC_CMD2[] =   {'A', '2'};  // Arduino to PC Command 2 TAG
char sp_ARDtoPC_CMD3[] =   {'A', '3'};  // Arduino to PC Command 3 TAG
// Arduino -> PC: TAGS
int ARDtoPC_param1;
int ARDtoPC_param2;
int ARDtoPC_param3;
// Arduino -> PC: FLAGS
boolean ARDtoPC_flag1;
boolean ARDtoPC_flag2;
boolean ARDtoPC_flag3;

boolean isConnected;

void  commSetup() {
  
  Serial.begin (SP_BAUDRATE);
  indexCommand = 0;
  haveCommand = false;  // We have a command from the PC
  isConnected = false;

  ARDtoPC_flag1 = false;
  ARDtoPC_flag2 = false;
  ARDtoPC_flag3 = false;
  PCtoARD_flag1 = false;
  PCtoARD_flag2 = false;
  PCtoARD_flag3 = false;
  
}

bool portIsConnected () {
  return (isConnected);  
}
  
void commManager () {
  
  // Serial port reception of commands from the PC
  while ( ( Serial.available() ) && ( haveCommand == false ) ) {
    pcCommand[indexCommand] = Serial.read();
    if ( pcCommand[indexCommand] == '\n' ) {  // Commands from the PC end with a \n
      haveCommand = true;      
    } else {
      ++indexCommand;
    }
  }

  if ( indexCommand == MAX_COMMAND ) {
    indexCommand = 0;
  }

  // PC -> Arduino  
  if ( haveCommand ) {
    haveCommand = false;
    if ( ( pcCommand[0] == sp_GO[0] ) && ( pcCommand[1] == sp_GO[1] ) && ( !isConnected ) ) {  // Turn on COM port communications
      isConnected = true;        
      Serial.println(sp_GO);
    } else if ( ( pcCommand[0] == sp_PCtoARD_CMD1[0] ) && ( pcCommand[1] == sp_PCtoARD_CMD1[1] ) && ( isConnected ) ) {   // Command 1 received
      PCtoARD_flag1 = true;
      PCtoARD_param1 = (int)((int)(pcCommand[3]-'0')*(int)1000+(int)(pcCommand[4]-'0')*(int)100+(int)(pcCommand[5]-'0')*(int)10+(int)(pcCommand[6]-'0'));
    } else if ( ( pcCommand[0] == sp_PCtoARD_CMD2[0] ) && ( pcCommand[1] == sp_PCtoARD_CMD2[1] ) && ( isConnected ) ) {  // Command 2 received
      PCtoARD_flag2 = true;
      PCtoARD_param2 = (int)((int)(pcCommand[3]-'0')*(int)1000+(int)(pcCommand[4]-'0')*(int)100+(int)(pcCommand[5]-'0')*(int)10+(int)(pcCommand[6]-'0'));
    } else if ( ( pcCommand[0] == sp_PCtoARD_CMD3[0] ) && ( pcCommand[1] == sp_PCtoARD_CMD3[1] ) && ( isConnected ) ) {  // Command 3 received
      PCtoARD_flag3 = true;
      PCtoARD_param3 = (int)((int)(pcCommand[3]-'0')*(int)1000+(int)(pcCommand[4]-'0')*(int)100+(int)(pcCommand[5]-'0')*(int)10+(int)(pcCommand[6]-'0'));
    } else {
    }   
    indexCommand = 0;
  }

  // Arduino -> PC    
  if ( ARDtoPC_flag1 ) {
      ARDtoPC_flag1 = false;
      sprintf(tmp_buf, "%c%c %d", sp_ARDtoPC_CMD1[0], sp_ARDtoPC_CMD1[1], ARDtoPC_param1);
      Serial.println(tmp_buf);
      }
  if ( ARDtoPC_flag2 ) {
      ARDtoPC_flag2 = false;
      sprintf(tmp_buf, "%c%c %d", sp_ARDtoPC_CMD2[0], sp_ARDtoPC_CMD2[1], ARDtoPC_param2);
      Serial.println(tmp_buf);
      }
  if ( ARDtoPC_flag3 ) {
      ARDtoPC_flag3 = false;
      sprintf(tmp_buf, "%c%c %d", sp_ARDtoPC_CMD3[0], sp_ARDtoPC_CMD3[1], ARDtoPC_param3);
      Serial.println(tmp_buf);
      }
  
  Serial.flush();

}

void sendData (int channel, int param) {

  if ( channel == CHANNEL_1 ) {
    ARDtoPC_flag1 = true;
    ARDtoPC_param1 = param;
  } else if ( channel == CHANNEL_2 ) {
    ARDtoPC_flag2 = true;
    ARDtoPC_param2 = param;    
  } else if ( channel == CHANNEL_3 ) {
    ARDtoPC_flag3 = true;
    ARDtoPC_param3 = param;    
  } else {
  }
  
}

bool dataAvailable (int channel) {

  boolean available;

  available = false;
  if ( channel == CHANNEL_1 ) {
    available = PCtoARD_flag1;
  } else if ( channel == CHANNEL_2 ) {
    available = PCtoARD_flag2;
  } else if ( channel == CHANNEL_3 ) {
    available = PCtoARD_flag3;
  } else {
  }

  return (available);
}

int getData (int channel) {
  int data;
  
  data = 0;
  if ( ( channel == CHANNEL_1 ) && ( PCtoARD_flag1 == true ) ) {
    data = PCtoARD_param1;
    PCtoARD_flag1 = false;    
  } else if ( ( channel == CHANNEL_2 ) && ( PCtoARD_flag2 == true ) ) {
    data = PCtoARD_param2;
    PCtoARD_flag2 = false;    
  } else if ( ( channel == CHANNEL_3 ) && ( PCtoARD_flag3 == true ) ) {
    data = PCtoARD_param3;
    PCtoARD_flag3 = false;    
  } else {
  }

  return (data);

}
