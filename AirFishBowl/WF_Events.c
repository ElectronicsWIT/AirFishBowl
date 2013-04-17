#include "WF_Events.h"

	
/****************************************************************************
  FUNCTIONS TO HANDLE THE WIFI EVENTS
****************************************************************************/

//	CONNECTION SUCCESSFUL
void OnConnectionSuccessful(UINT8 ev, UINT16 evinfo)
{  
	IOPut (o5,on);
	#if defined(STACK_USE_UART)
	UARTWrite(1,"Event: Connection Successful\r\n"); 
	#endif
}

//	CONNECTION FAILED
void OnConnectionFailed(UINT8 ev, UINT16 evinfo)
{
	#if defined(STACK_USE_UART)
	char buf[8];
	UARTWrite(1,"Event: Connection Failed  -- eventInfo = ");
	sprintf(buf, "%d\r\n", evinfo);
	UARTWrite(1,buf);	
	#endif
}

//	CONNECTION TEMPORARLY LOST
void OnConnectionTempLost(UINT8 ev, UINT16 evinfo)
{
	#if defined(STACK_USE_UART)
	char buf[8];
	UARTWrite(1,"Event: Connection Temporarily Lost -- eventInfo = ");
	sprintf(buf, "%d\r\n", evinfo);
	UARTWrite(1,buf);
	#endif
}

//CONNECTION REESTABLISHED
void OnConnectionReestablished(UINT8 ev, UINT16 evinfo)
{
	#if defined(STACK_USE_UART)
	UARTWrite(1,"Event: Connection Reestablished\r\n");
	#endif
}
//	CONNECTION PERMANENTLY LOST
void OnConnectionPermLost(UINT8 ev, UINT16 evinfo)
{
	IOPut (o5,off);
	#if defined(STACK_USE_UART)    
	char buf[8];   
	UARTWrite(1,"Event: Connection Permanently Lost -- eventInfo = ");
	sprintf(buf, "%d\r\n", evinfo);
	UARTWrite(1,buf);
	#endif
}

//	SCAN RESULT
void OnScanResult(UINT8 ev, UINT16 evinfo)
{
	#if defined(STACK_USE_UART)
	char buf[8];
	UARTWrite(1,"Event: Scan Results Ready,");
	sprintf(buf, "%d", evinfo);
	UARTWrite(1,buf);
	UARTWrite(1,"results\r\n");
	#endif
}

