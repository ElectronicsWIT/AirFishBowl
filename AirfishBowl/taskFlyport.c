#include "taskFlyport.h"

void FlyportTask()
{

WFConnect(WF_DEFAULT);
while (WFStatus != CONNECTED);
UARTWrite(1,"Flyport connected... hello world!\r\n");	

extern int myGlobal;
extern int myGlobal2;

const float left = 8.25; //here we set max   2ms
const float right = 4; //and here the min  1ms
const float middle = 6.15; // 1.23ms
const float stop = 5.15; // 5.15ms
const float down = 8; // 8ms

float work = (float)left;
float work2 = (float)left;
PWMInit(1, 50,left);
PWMInit(2, 50,left);
PWMOn(d1out, 1);
PWMOn(d4out, 2);



while(1)
{
	if((myGlobal == 0))
	{
		//forward
		for (work = left; work > right; work--)
			{
			PWMDuty(work, 1);
			vTaskDelay(5); //used to slow down the effect
			}
					vTaskDelay(50);
		for (work = right; work < left; work ++)
			{
				PWMDuty(work, 1);
				vTaskDelay(5); //used to slow down the effect
			}
			vTaskDelay(50); 
	}
	
	if((myGlobal == 1))
	{
		//Stop
		
		PWMDuty(middle, 1);
		vTaskDelay(10); //used to slow down the effect
		
	}
	
	if((myGlobal == 2))
	{
		//Left
		for (work = middle; work < left; work++)
			{
			PWMDuty(work, 1);
			vTaskDelay(15); //used to slow down the effect
			}
		for (work = left; work > middle; work --)
			{
				PWMDuty(work, 1);
				vTaskDelay(15); //used to slow down the effect
			}
	}
	
	
	if((myGlobal == 3))
	{
		//right
		for (work = middle; work > right; work--)
			{
			PWMDuty(work, 1);
			vTaskDelay(15); //used to slow down the effect
			}
		for (work = right; work < middle; work ++)
			{
				PWMDuty(work, 1);
				vTaskDelay(15); //used to slow down the effect
			}
	}
	
	
	
	if((myGlobal2 == 0))
	{
	
	}
	if((myGlobal2 == 4))
	{
		//Up
		    work2 > right, work2--;//work2 = left - 0.5;
			
			PWMDuty(work2, 2);
			//vTaskDelay(5); //used to slow down the effect
		/*for (work = right; work < middle; work ++)
			{
				PWMDuty(work, 2);
				vTaskDelay(15); //used to slow down the effect
			}*/
			myGlobal2 = 0;
			
	}
	
	
	
	if((myGlobal2 == 5))
	{
		//Down = 8ms
		work2 = down; 
		//work2 < down; //work2++) 
		PWMDuty(work2, 2);
		vTaskDelay(30); //used to slow down the effect
		
		/*for (work2 = right; work2 < middle; work2 ++)
			{
				PWMDuty(work2, 2);
				vTaskDelay(15); //used to slow down the effect
			}*/
	}
	
	
	
	
	
	
}

}