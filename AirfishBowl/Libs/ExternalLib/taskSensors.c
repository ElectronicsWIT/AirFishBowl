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
		UARTWrite(1,"Executing Sensor task\r\n");
		
		if(i > 100)
			i = 0;
		
		
		i++;
		
		// Print out sample message type to UART
		sprintf(str, "Value %d\r\n", i);
		UARTWrite(1, str);
		
		// Print out sample data to UART
		sprintf(str, "Value %f\r\n", sampledata);
		UARTWrite(1, str);		
		
		// Populating our packet structure with the message type and the data
		msg.message_type = i;
		msg.message_data = sampledata;
		
		// Writing packet structure to the Flyport Queue
		xQueueSendToBack(xFlyportQueue, ( void * ) &msg, ( portTickType ) 1);
		
		UARTWrite(1,"Sensor task Complete\r\n");
	}
}
