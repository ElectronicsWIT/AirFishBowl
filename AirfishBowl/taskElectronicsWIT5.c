#include "taskElectronicsWIT5.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan, Patrick Cody
*/

void ElectronicsWITTask5()
{
	while(1){
		vTaskSuspendAll();	
		UARTWrite(1,"vTaskSuspendAll task 5\r\n");
		
		IOPut(p19, toggle);
		IOPut(p21, toggle);
		
		vTaskDelay(100);
		
		UARTWrite(1,"xTaskResumeAll task 5\r\n");
		xTaskResumeAll();
	}
}
