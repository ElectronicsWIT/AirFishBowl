/*
*	Message Types Assumptions:
*	0 = stop
*	1 = swim
*	2 = follow
*	3 = avoid
*	4 = feed
*	5 = Manual
*
*	//9 = sensor Data// IGNORE
*/
var state = 0;
/*window.setInterval(function(){alive()},500);*/
var busy = 0;


/***** Sensor Data *****/
var sensor_height;

/*************************/

function refreshData()
{
	//sensor_height = // JQuery to get sensor value from webpage
}

function alive()
{
	//console.log("Loop");
	switch(state)
	{
		case 0:
			console.log("Stopped");
			//stop();
		break;
		
		case 1:
			if (busy == 0)
			{
				//console.log("Swimming");
				swim();
			}
		break;
		
		case 2:
			//NOT Implemented
			follow();
		break;
		
		case 3:
			//NOT Implemented
			avoid();
		break;
		
		case 4:
			//NOT Implemented
			feed();
		break;
		
		case 5:
			//DoNothingCozManual!!!!
			return;
		break;
		
		default:
			state = 0;
	}
}


/***** Idle Automation *****/
action = 0;
cycles = 0;

function swim()
{
var prevAction = action;
	// Needed Swim movement action times
	/*
	* 	Forward time 	= ?
	* 	Right time 		= ?
	* 	Left time 		= ?
	* 	Up time 		= ?
	* 	Down time 		= ?
	*/
	/***** Control Values *****/
	// Forward 	= 0
	// Stop		= 1
	// Left 	= 2
	// Right	= 3
	// Up		= 4
	// Down 	= 5
	busy = 1;
	console.log("Action: " +action);
	console.log("Cycles: " +cycles);
	switch(action)
	{
		case 0:
			cycles++;
			newAJAXCommand('leds.cgi?led=0');
			console.log("forward");
			
			if (cycles > 1 && cycles < 3)
			{
				choices = [0,2,3];
				action = choices[getRandom(0,2,1)];
			}
			else if (cycles >= 3)
			{
				choices = [2,3];
				action = choices[getRandom(0,1,1)];
			}
			setTimeout(function(){busy = 0;},getRandom(1000,1500,0));
			
			if (action != prevAction)
			{
				cycles = 0;
			}
		break;
		
		case 2:
			cycles++;
			newAJAXCommand('leds.cgi?led=2');
			console.log("left");
			
			if (cycles < 3)
			{
				choices = [0,2];
				action = choices[getRandom(0,1,1)];
			}
			else if (cycles >= 3)
			{
				action = 0;
			}
			setTimeout(function(){busy = 0;},getRandom(1000,1500,0));
			
			if (action != prevAction)
			{
				cycles = 0;
			}
		break;
		
		case 3:
			cycles++;
			newAJAXCommand('leds.cgi?led=3');
			console.log("right");
			
			if (cycles < 3)
			{
				choices = [0,3];
				action = choices[getRandom(0,1,1)];
			}
			else if (cycles >= 3)
			{
				action = 0;
			}
			setTimeout(function(){busy = 0;},getRandom(1000,1500,0));
			
			if (action != prevAction)
			{
				cycles = 0;
			}
		break;
		}
}

function getRandom (min, max, round) {
	if (round == 0)
	{
		return Math.random() * (max - min) + min;
	}
	else
		return Math.round(Math.random() * (max - min) + min);
}

function getSafe()
{
	// Go down untill height off ground is < 1.5 meters
	// then stop
}

function setState(number)
{
	state = number;
}

/***** Idle Timer *****/
var idleTime = 0;
$(document).ready(function () {
    //Increment the idle time counter every second.
    var idleInterval = setInterval("timerIncrement()", 1000); // 1 second

    //Zero the idle timer on fish movement or movment button press
    $(this).keypress(function (e) {
        idleTime = 0;
    });
});
function timerIncrement() {
    idleTime = idleTime + 1;
    if (idleTime > 29) { // 20 minutes
        state = 1;
    }
}
/*************************/