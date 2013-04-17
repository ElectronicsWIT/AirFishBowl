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
	//Forward Movement
	if((myGlobal == 0))
	{
	 for (work = left; work > right; work--)
		{
		PWMDuty(work, 1);
		vTaskDelay(5); //used to slow down the effect
		}
		vTaskDelay(15);
	 for (work = right; work < left; work ++)
		{
		PWMDuty(work, 1);
		vTaskDelay(5); //used to slow down the effect
		}
		vTaskDelay(15); 
	}
	
	//Stop 
	if((myGlobal == 1))
	{
	PWMDuty(middle, 1);
	vTaskDelay(10); //used to slow down the effect
	}
	
	//Left Movement
	if((myGlobal == 2))
	{
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
	
	//Right Movement
	if((myGlobal == 3))
	{
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
	//Dummy function
	}
	
	//Ballast Control - Up Movement
	if(myGlobal2 == 4)
	{
		if(work2 > right)
			work2--;//work2 = left - 0.5;
			
		PWMDuty(work2, 2);
		myGlobal2 = 0;
	}
	
	//Ballast Control - Down Movement
	if((myGlobal2 == 5))
	{
		//Down = 8ms
		work2 = down; 
		//work2 < down; //work2++) 
		PWMDuty(work2, 2);
		vTaskDelay(30); //used to slow down the effect
	}
	
}

}