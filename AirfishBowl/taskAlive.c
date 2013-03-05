#include "taskAlive.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/

void AliveTask()
{
	Fishmsg msg;
	int i;
	//char str[10];
	while(1){	
		//UARTWrite(1,"Alive task\r\n");
		
		i = 0;
		xQueueReceive(xAliveQueue,&msg,0);
		i = msg.message_type;
		
		//sprintf(str, "Value %d\r\n", i);
		//UARTWrite(1, str);
		
		xQueueSendToBack(xMoveQueue, ( void * ) &msg, ( portTickType ) 50);
		
		//UARTWrite(1,"Alive task complete\r\n");
	}
}

