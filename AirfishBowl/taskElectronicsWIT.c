#include "taskElectronicsWIT.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/

void ElectronicsWITTask()
{
	int i;
	while(1){
		vTaskSuspendAll();	
		UARTWrite(1,"vTaskSuspendAll task 1\r\n");
		
		i = 0;
		
		xQueueReceive(ourQueueStruct.one,&i,0);
		
		if(i == 1)
		{
			IOPut(p19, toggle);
			IOPut(p21, toggle);
			xQueueSendToBack(ourQueueStruct.two, ( void * ) &i, ( portTickType ) 50);
		}
		
		vTaskDelay(100);
		
		UARTWrite(1,"xTaskResumeAll task 1\r\n");
		xTaskResumeAll();
	}
}
