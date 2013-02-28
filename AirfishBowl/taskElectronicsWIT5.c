#include "taskElectronicsWIT5.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/
/*
void ElectronicsWITTask5()
{
	int i;
	while(1){
		vTaskSuspendAll();	
		UARTWrite(1,"vTaskSuspendAll task 5\r\n");
		
		i = 0;
		
		xQueueReceive(ourQueueStruct.five,&i,0);
		
		if(i == 1)
		{
			IOPut(p19, toggle);
			IOPut(p21, toggle);
			xQueueSendToBack(ourQueueStruct.one, ( void * ) &i, ( portTickType ) 50);
		}
		
		vTaskDelay(100);
		
		UARTWrite(1,"xTaskResumeAll task 5\r\n");
		xTaskResumeAll();
	}
}
*/
