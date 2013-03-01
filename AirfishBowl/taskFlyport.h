#include "INTlib.h"

#include "string.h"
#include "TCPlib.h"
#include "UDPlib.h"
#include "NETlib.h"
#include "SMTPlib.h"
#include "FTPlib.h"

//	RTOS components - Semaphore and queues
extern xQueueHandle xQueue;
extern xSemaphoreHandle xSemFrontEnd;
extern xSemaphoreHandle xSemHW;


//	FrontEnd variables
extern BYTE xIPAddress[];
extern WORD xTCPPort;
extern TCP_SOCKET xSocket;
extern int xFrontEndStat;
extern int xErr;
extern BOOL xBool;
extern WORD xWord;
extern char *xChar;
extern BYTE *xByte;
extern BYTE xByte2;
extern BYTE xByte3;
extern NODE_INFO xNode;

extern SMTP_POINTERS SMTPClient;
extern BOOL DHCPAssigned;
#if defined (FLYPORTETH)
extern BOOL MACLinked;
#endif

void FlyportTask();

