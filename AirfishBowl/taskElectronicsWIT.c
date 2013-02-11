#include "taskElectronicsWIT.h"

/*
	Applied Electronics Y4, Applied Operating Systems Y4
	AirfishBowl project
	David Kirwan, Patrick Cody
*/

void AppliedRealTimeTask()
{

	while(1){
		UARTWrite(1,"Inside our task\r\n");
		IOPut(p19, toggle);
		IOPut(p21, toggle);
		vTaskDelay(100);
		UARTWrite(1,"After our task loop\r\n");
	}

}