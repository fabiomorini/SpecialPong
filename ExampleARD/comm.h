#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3

void  commSetup();
bool portIsConnected ();
void commManager ();
void sendData (int channel, int param);
bool dataAvailable (int channel);
int getData (int channel);
