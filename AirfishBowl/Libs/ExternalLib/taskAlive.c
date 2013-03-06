#include "taskAlive.h"

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/

/*
*	Message Types Assumptions:
*	1 = swim
*	2 = follow
*	3 = avoid
*	4 = feed
*	5 = Manual
*
*	9 = sensor Data
*/

void AliveTask()
{
	int alive = 0;
	int swim = 0;
	int avoid = 0;
	int follow = 0;
	int feed = 0;
	Fishmsg msg;
	int i;
	//char str[10];
	while(1){	
		//UARTWrite(1,"Alive task\r\n");
		
		i = 0;
		xQueueReceive(xAliveQueue,&msg,0);
		i = msg.message_type;
		
		//sprintf(str, "Value %d\r\n", i);
		//UARTWrite(1, str);
		if (i == 9)
		{
			//grab sensor data and do somethign with it
		}
		else if (i == 0 && alive != 0)
		{
			if (swim != 0){
				DoSwim();
			}
			if (avoid != 0){
				DoAvoid();
			}
			if (follow != 0){
				DoFollow();
			}
			if (feed != 0){
				DoFeed();
			}
		}
		else
		{
			alive = 1;
			
			if (i == 5)
			{
				alive = ManualControl();
			}
		}
		
		//xQueueSendToBack(xMoveQueue, ( void * ) &msg, ( portTickType ) 50);
		
		//UARTWrite(1,"Alive task complete\r\n");
		
		
		/*
		xQueueHandle xMoveQueue;
		xQueueHandle xAliveQueue;
		xQueueHandle xFlyportQueue;
		*/
	}
}

void DoSwim()
{
	//Implement Swim Behaviour
}

void DoAvoid()
{
	//Implement Avoid Behaviour
}

void DoFeed()
{
	//Implement Feed Behaviour
}

void DoFollow()
{
	//Implement Follow Behaviour
}

int ManualControl()
{
	return 0;
}

void GetSafe()
{
	//Implement Swim Behaviour
}
