#include "taskSensors.h"

#define	FRONT_DIST_MEAS	2001
#define	LS_DIST_MEAS	2002
#define	RS_DIST_MEAS	2003
#define HEIGHT_MEAS		2004
#define	CURR_MEAS		2005
#define VOLT_MEAS		2006

double ReadDist(int ADCPin);
double ReadPower(int num);

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan, Glenn Gall, Kieran Barrett
*/

void SensorsTask()
{
	Fishmsg height, current;
	double measVal;
	
	while(1){
		// get height measurement
		measVal = ReadDist(3);	// height sensor connected to ADC3 input (pin 20)
		height.message_type = HEIGHT_MEAS;
		height.message_data = measVal;
		xQueueSendToBack(xFlyportQueue, ( void * ) &height, ( portTickType ) 1);
		
		// get current measurement
		measVal = ReadPower(1);	// case 1 is current
		current.message_type = CURR_MEAS;
		current.message_data = measVal;
		xQueueSendToBack(xFlyportQueue, ( void * ) &current, ( portTickType ) 1);
		
		vTaskDelay(10);
	}
}

double ReadDist(int ADCPin)
{
	int DistVal = ADCVal(ADCPin);
    
    double cm = (DistVal * 0.002) / ( (3.3 / 512) / 2.54 );
    
    return cm;
}

double ReadPower(int num)
{
	double ADval;
	int reading = 0;
	int total = 0;
	int counter = 0;
	int average;
	int answer = 0;
	

	// Analog values reading according to the webpage request.
	switch(num)
	{
		case 1:
			for(counter= 0; counter<100; counter++)
			{
			  reading = ADCVal(1);
			  total = total + reading;
			}
			average = total/(counter+1);
			answer = (average-186)*2;
			
			ADval = answer;
		break;
		case 2:
			ADval = ADCVal(2)*4;
		break;	
	}
	
	return ADval;
}
