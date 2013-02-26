#include "taskElectronicsWIT2.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan,
*/

void ElectronicsWITTask2()
{
	int i;
	while(1){
		vTaskSuspendAll();	
		UARTWrite(1,"vTaskSuspendAll task 2\r\n");
		
		i = 0;
		
		xQueueReceive(ourQueueStruct.two,&i,0);
		
		if(i == 1)
		{
			IOPut(p19, toggle);
			IOPut(p21, toggle);
			xQueueSendToBack(ourQueueStruct.three, ( void * ) &i, ( portTickType ) 50);
		}
		
		vTaskDelay(100);
		
		UARTWrite(1,"xTaskResumeAll task 2\r\n");
		xTaskResumeAll();
	}
}
