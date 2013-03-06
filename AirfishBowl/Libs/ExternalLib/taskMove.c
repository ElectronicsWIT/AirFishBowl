#include "taskMove.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/

void MoveTask()
{
	Fishmsg msg;
	int i;
	//char str[10];
	while(1){	
		//UARTWrite(1,"Move task\r\n");
		
		i = 0;
		xQueueReceive(xMoveQueue,&msg,0);
		i = msg.message_type;
		
		//sprintf(str, "Value %d\r\n", i);
		//UARTWrite(1, str);
		
		//UARTWrite(1,"xTaskResumeAll Move task\r\n");
	}
}
