function arrows_bk_default() {
	$('.controller-right').removeClass('pressed-left');
	$('.controller-right').removeClass('pressed-right');
	$('.controller-right').removeClass('pressed-up');
	$('.controller-right').removeClass('pressed-down');
}

$(document).ready(function() {

$(document).keydown(function(e){
    if (e.keyCode == 37) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-left');
       $('.controller-arrows-left').addClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 38) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-up');
       $('.controller-arrows-up').addClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 39) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-right');
       $('.controller-arrows-right').addClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 40) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-down');
       $('.controller-arrows-down').addClass('controller-arrows-pressed');
       return false;
    }
});

$(document).keyup(function(e){
    if (e.keyCode == 37) {
       $('.controller-right').removeClass('pressed-left');
       $('.controller-arrows-left').removeClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 38) { 
       $('.controller-right').removeClass('pressed-up');
       $('.controller-arrows-up').removeClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 39) { 
       $('.controller-right').removeClass('pressed-right');
       $('.controller-arrows-right').removeClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 40) { 
       $('.controller-right').removeClass('pressed-down');
       $('.controller-arrows-down').removeClass('controller-arrows-pressed');
       return false;
    }
});

});

