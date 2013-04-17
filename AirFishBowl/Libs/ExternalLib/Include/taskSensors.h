#include "INTlib.h"
#include "HWlib.h"

#ifndef SENSORS_ELECTRONICS_WIT
#define SENSORS_ELECTRONICS_WIT

#include "utilitiesElectronicsWIT.h"

extern xQueueHandle xFlyportQueue;

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/

// Function prototype
void SensorsTask();
double ReadDist(int ADCPin);
double ReadPower(int choice);

#endif
