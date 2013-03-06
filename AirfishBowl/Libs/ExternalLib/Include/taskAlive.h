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


void DoSwim();
void DoAvoid();
void DoFollow();
void DoFeed();
int ManualControl();
void GetSafe();
