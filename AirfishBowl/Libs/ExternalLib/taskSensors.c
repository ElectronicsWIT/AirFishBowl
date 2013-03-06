#include "taskSensors.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan,
*/

void SensorsTask()
{
	Fishmsg msg;
	int i = 0;
	double sampledata = 25.5;
	char str[10];
	
	while(1){
		//UARTWrite(1,"Sensor task\r\n");
		
		if(i > 100)
			i = 0;
		
		
		i++;
		
		sprintf(str, "Value %d\r\n", i);
		UARTWrite(1, str);
		
		//sprintf(str, "Value %f\r\n", sampledata);
		//UARTWrite(1, str);		
		
		msg.message_type = i;
		msg.message_data = sampledata;
		
		xQueueSendToBack(xFlyportQueue, ( void * ) &msg, ( portTickType ) 1);
		xQueueSendToBack(xAliveQueue, ( void * ) &msg, ( portTickType ) 1);
		
		//UARTWrite(1,"Sensor task Complete\r\n");
	}
}
