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
window.setInterval(function(){alive()},5000);
var busy = 0;
/*
*	After a timeout of about 30 seconds of no user input the fish should automate.
*
*
*
*
*/

function alive()
{
	console.log("Loop");
	switch(state)
	{
		case 0:
			console.log("Stopped");
			//stop();
		break;
		
		case 1:
			if (busy == 0)
			{
				console.log("Swimming");
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
		break;
		
		default:
			state = 1;
	}
}


function swim()
{
	busy = 1;
	newAJAXCommand('leds.cgi?led=0');
	setTimeout(function(){newAJAXCommand('leds.cgi?led=0');},2000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=0');},3000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=0');},5000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=2');},7000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=2');},9000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=2');},11000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=0');},12000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=0');},13000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=0');},14000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=2');},16000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=2');},18000);
	setTimeout(function(){newAJAXCommand('leds.cgi?led=2');},20000);
	setTimeout(function(){busy=0;},22000);
	//newAJAXCommand('leds.cgi?led=0'); //Forward
	
	//newAJAXCommand('leds.cgi?led=2'); //Left
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