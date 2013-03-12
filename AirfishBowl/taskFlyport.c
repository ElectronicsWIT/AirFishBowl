#include "taskFlyport.h"
#include "utilitiesElectronicsWIT.h"

void FlyportTask()
{
	WFConnect(WF_DEFAULT);
	while (WFStatus != CONNECTED);
	UARTWrite(1,"Flyport connected... hello world!\r\n");

	Fishmsg msg;
	int i = 0;
	char str[10];
	while(1){
		UARTWrite(1,"Executing Flyport task\r\n");
		
		i = 0;
		xQueueReceive(xFlyportQueue,&msg,0);
		i = msg.message_type;
		
		sprintf(str, "Value %d\r\n", i);
		UARTWrite(1, str);
		
		UARTWrite(1,"Flyport task complete\r\n");
	}
}
