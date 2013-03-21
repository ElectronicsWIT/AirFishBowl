var clearID = 0;

/*****************Iphone detection

if((navigator.userAgent.match(/iPhone/i)) || (navigator.userAgent.match(/iPod/i))) {
   if (document.cookie.indexOf("iphone_redirect=false") == -1) window.location = "http://m.espn.go.com/wireless/?iphone&i=COMR";
}

**************************************/


/**function ButtonPress (event){
            var keyCode = ('which' in event) ? event.which : event.keyCode;
            //alert ("The Unicode key code is: " + keyCode);
			switch(event.keyCode)
		{
			//case 38:
			//newAJAXCommand('leds.cgi?led=0');
			//break;
			case 87: //W
			$('#up').toggleClass("hovered");
			$('#up').click();
			//newAJAXCommand('leds.cgi?led=0');
			break;
			case 65: //A
			$('#left').click();
			$('#left').toggleClass("hovered");
			//newAJAXCommand('leds.cgi?led=1');
			break;
			case 51: //S
			$('#down').click();
			$('#down').toggleClass("hovered");
			//newAJAXCommand('leds.cgi?led=2');
			break;
			case 68: //D
			$('#right').click();
			$('#right').toggleClass("hovered");
			//newAJAXCommand('leds.cgi?led=3');
			break;
			case 32:
			$('#swim').toggleClass("hovered");
			$('#stop').toggleClass("hovered");
			break;
		}
}

function controlLoop(){
        clearTimeout(clearID);
        // Check for right motion
        if (k_right){
			$('#right').click();
			
        }
        // Check for left motion
        else if (k_left){
            $('#left').click();
			
        }
        // Check for forward motion
        else if (k_up){
            
			$('#up').click();
        }
        // Else tell the fish to stop
        else {
               
        }
        // loop :D

        clearID = setTimeout("controlLoop()",100);
}**/

document.ontouchmove = function(e){ e.preventDefault(); }

/*************************Keyboard Control ***********************************/

var k_up = 0;
var k_left = 0;
var k_right = 0;
var k_down = 0;

function keyDown (event){
	//alert ("The Unicode key code is: " + event.keyCode);
       
        if (event.keyCode == 65){
                k_left = 1;
				$('#left').addClass("hovered");
				newAJAXCommand('leds.cgi?led=2');
                //controlLoop();
        }

        else if (event.keyCode == 68){
                k_right = 1;
				$('#right').addClass("hovered");
				newAJAXCommand('leds.cgi?led=3');
                //controlLoop();
        }
		
		else if (event.keyCode == 83){
                k_down = 1;
				$('#down').addClass("hovered");
				newAJAXCommand('leds.cgi?led=5');
                //controlLoop();
        }
		else if (event.keyCode == 87){
                k_up = 1;
				$('#up').addClass("hovered");
				newAJAXCommand('leds.cgi?led=4');
                //controlLoop();
        }
		
		else if (event.keyCode == 32){
            $('#swim').toggleClass("hovered");
			$('#stop').toggleClass("hovered");
			if ($('#swim').hasClass("hovered"))
			{
				newAJAXCommand('leds.cgi?led=0');
			}
			if ($('#stop').hasClass("hovered"))
			{
				newAJAXCommand('leds.cgi?led=1');
			}
			
        }
}

function keyUp (event){
        if (event.keyCode == 87){
                k_up = 0;
				$('#up').removeClass("hovered");
				newAJAXCommand('leds.cgi?led=4');
                //controlLoop();
        }

        else if (event.keyCode == 65){
                k_left = 0;
				$('#left').removeClass("hovered");
				newAJAXCommand('leds.cgi?led=2');
                //controlLoop();
        }

        else if (event.keyCode == 68){
                k_right = 0;
				$('#right').removeClass("hovered");
				newAJAXCommand('leds.cgi?led=3');
                //controlLoop();
        }
		
		else if (event.keyCode == 83){
                k_down = 0;
				$('#down').removeClass("hovered");
				newAJAXCommand('leds.cgi?led=5');
                //controlLoop();
        }

}


window.addEventListener("keydown", function(event){ keyDown (event) });
window.addEventListener("keyup", function(event){ keyUp (event) });

/******************************************End Keyboard Control********************************/



/***************************************Mouse control*************************************/
var m_swim = 0;
var m_left = 0;
var m_right = 0;
var m_up = 0;
var m_down = 0;


function mouseButtonClick(e){
        if (e.target.id == "swim"){
                //m_swim = 1;
				$('#swim').addClass("hovered");
				$('#stop').removeClass("hovered");
		
				$('#auto').removeClass("hovered");
				newAJAXCommand('leds.cgi?led=0');
                //controlLoop();
        }
		
		else if (e.target.id == "auto"){
                //m_swim = 1;
				$('#swim').removeClass("hovered");
				$('#stop').removeClass("hovered");
				$('#auto').addClass("hovered");
				setState(1);
                //controlLoop();
        }
		
		else if (e.target.id == "stop"){
                //m_left = 1;
				$('#stop').addClass("hovered");
				$('#swim').removeClass("hovered");
				$('#auto').removeClass("hovered");
				newAJAXCommand('leds.cgi?led=1');
				setState(0);
                //controlLoop();
        }

        else if (e.target.id == "left"){
                //m_left = 1;
				$('#left').addClass("hovered");
				newAJAXCommand('leds.cgi?led=2');
                //controlLoop();
        }

        else if (e.target.id == "right"){
                //m_right = 1;
				$('#right').addClass("hovered");
				newAJAXCommand('leds.cgi?led=3');
                //controlLoop()
        }
		
		else if (e.target.id == "up"){
                //m_up = 1;
				$('#up').addClass("hovered");
				newAJAXCommand('leds.cgi?led=4');
                //controlLoop()
        }
		
		else if (e.target.id == "down"){
                //m_down = 1;
				$('#down').addClass("hovered");
				newAJAXCommand('leds.cgi?led=5');
                //controlLoop()
        }
}

function mouseButtonRelease(){
        $('#up').removeClass("hovered");
		$('#down').removeClass("hovered");
		$('#left').removeClass("hovered");
		$('#right').removeClass("hovered");
}

window.addEventListener("mousedown", function(event){ mouseButtonClick(event) } );
window.addEventListener("mouseup", mouseButtonRelease);

/**********************************************End Mouse Control********************************/

/**************************** iPhone Touch Detection *******************************************/

window.addEventListener('touchstart', function(event){ keyDown (event) } );
window.addEventListener('touchend', mouseButtonRelease );

/**************************************************************************************************/


window.addEventListener('load', function() {
    setTimeout(scrollTo, 0, 0, 1);
}, false);
