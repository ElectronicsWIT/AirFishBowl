function arrows_bk_default() {
	$('.controller-right').removeClass('pressed-left');
	$('.controller-right').removeClass('pressed-right');
	$('.controller-right').removeClass('pressed-up');
	$('.controller-right').removeClass('pressed-down');
}

var isSwimming = 0;

$(document).ready(function() {

/* Control of keyboard events */
$(document).keydown(function(e){
    if (e.keyCode == 37) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-left');
       $('.controller-arrows-left').addClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=2');

       return false;
    }
    if (e.keyCode == 38) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-up');
       $('.controller-arrows-up').addClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=4');

       return false;
    }
    if (e.keyCode == 39) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-right');
       $('.controller-arrows-right').addClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=3');

       return false;
    }
    if (e.keyCode == 40) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-down');
       $('.controller-arrows-down').addClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=5');

       return false;
    }
});
$(document).keyup(function(e){
    if (e.keyCode == 37) {
       $('.controller-right').removeClass('pressed-left');
       $('.controller-arrows-left').removeClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=2');

       return false;
    }
    if (e.keyCode == 38) { 
       $('.controller-right').removeClass('pressed-up');
       $('.controller-arrows-up').removeClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=4');

       return false;
    }
    if (e.keyCode == 39) { 
       $('.controller-right').removeClass('pressed-right');
       $('.controller-arrows-right').removeClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=3');

       return false;
    }
    if (e.keyCode == 40) { 
       $('.controller-right').removeClass('pressed-down');
       $('.controller-arrows-down').removeClass('controller-arrows-pressed');

       newAJAXCommand('leds.cgi?led=5');

       return false;
    }
});

/* Control click event of arrows */
/* Left arrow click */
$('.controller-arrows-left').mousedown(function() {
	$('.controller-right').addClass('pressed-left');
    $('.controller-arrows-left').addClass('controller-arrows-pressed');
});
$('.controller-arrows-left').mouseup(function() {
	$('.controller-right').removeClass('pressed-left');
    $('.controller-arrows-left').removeClass('controller-arrows-pressed');
});
/* Up arrow click */
$('.controller-arrows-up').mousedown(function() {
	$('.controller-right').addClass('pressed-up');
    $('.controller-arrows-up').addClass('controller-arrows-pressed');
});
$('.controller-arrows-up').mouseup(function() {
	$('.controller-right').removeClass('pressed-up');
    $('.controller-arrows-up').removeClass('controller-arrows-pressed');
});
/* Right arrow click */
$('.controller-arrows-right').mousedown(function() {
	$('.controller-right').addClass('pressed-right');
    $('.controller-arrows-right').addClass('controller-arrows-pressed');
});
$('.controller-arrows-right').mouseup(function() {
	$('.controller-right').removeClass('pressed-right');
    $('.controller-arrows-right').removeClass('controller-arrows-pressed');
});
/* Down arrow click */
$('.controller-arrows-down').mousedown(function() {
	$('.controller-right').addClass('pressed-down');
    $('.controller-arrows-down').addClass('controller-arrows-pressed');
});
$('.controller-arrows-down').mouseup(function() {
	$('.controller-right').removeClass('pressed-down');
    $('.controller-arrows-down').removeClass('controller-arrows-pressed');
});


/* Old Controllers */

function keyDown (event){      
    if (event.keyCode == 65){}
        newAJAXCommand('leds.cgi?led=2');
    }
    else if (event.keyCode == 68){
        newAJAXCommand('leds.cgi?led=3');
    }
    else if (event.keyCode == 83){
        newAJAXCommand('leds.cgi?led=5');
    }
    else if (event.keyCode == 87){
        newAJAXCommand('leds.cgi?led=4');
    }
    else if (event.keyCode == 32){
      if (isSwimming == 0){
        newAJAXCommand('leds.cgi?led=0');
        isSwimming = 1;
      }
      if (isSwimming == 1){
        newAJAXCommand('leds.cgi?led=1');
        isSwimming = 0;
      }     
    }
}

function keyUp (event){
    if (event.keyCode == 87){
        newAJAXCommand('leds.cgi?led=4');
                //controlLoop();
    }

    else if (event.keyCode == 65){
        newAJAXCommand('leds.cgi?led=2');
    }

    else if (event.keyCode == 68){
        newAJAXCommand('leds.cgi?led=3');
    }
    
    else if (event.keyCode == 83){
        newAJAXCommand('leds.cgi?led=5');
    }
}

window.addEventListener("keydown", function(event){ keyDown (event) });
window.addEventListener("keyup", function(event){ keyUp (event) });



});
