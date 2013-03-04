#include "INTlib.h"
#include "HWlib.h"
#include "utilitiesElectronicsWIT.h"

extern xQueueHandle xAliveQueue;
extern xQueueHandle xMoveQueue;

/*
	Applied Electronics, Applied Operating Systems
	David Kirwan
*/

// Function prototype
void AliveTask();
