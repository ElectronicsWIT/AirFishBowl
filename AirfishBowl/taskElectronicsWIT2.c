#include "taskElectronicsWIT2.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan, Patrick Cody
*/

void ElectronicsWITTask2()
{
	while(1){
		vTaskSuspendAll();	
		UARTWrite(1,"vTaskSuspendAll task 2\r\n");
		
		IOPut(p19, toggle);
		IOPut(p21, toggle);
		
		vTaskDelay(100);
		
		UARTWrite(1,"xTaskResumeAll task 2\r\n");
		xTaskResumeAll();
	}
}
