#include "taskElectronicsWIT3.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/
/*
void ElectronicsWITTask3()
{
	int i;
	while(1){
		vTaskSuspendAll();	
		UARTWrite(1,"vTaskSuspendAll task 3\r\n");
		
		i = 0;
		
		xQueueReceive(ourQueueStruct.three,&i,0);
		
		if(i == 1)
		{
			IOPut(p19, toggle);
			IOPut(p21, toggle);
			xQueueSendToBack(ourQueueStruct.four, ( void * ) &i, ( portTickType ) 50);
		}
		
		vTaskDelay(100);
		
		UARTWrite(1,"xTaskResumeAll task 3\r\n");
		xTaskResumeAll();
	}
}
*/
