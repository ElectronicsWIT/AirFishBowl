#include "queue.h"

#ifndef UTILITIES_ELECTRONICS_WIT
#define UTILITIES_ELECTRONICS_WIT
/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/

typedef struct EWITQueueStruct
{
    xQueueHandle *one;
	xQueueHandle *two;
	xQueueHandle *three;
	xQueueHandle *four;
	xQueueHandle *five;
}ElectronicsWITQueueStruct;

#endif
