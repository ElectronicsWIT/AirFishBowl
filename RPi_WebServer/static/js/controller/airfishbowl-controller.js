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
       return false;
    }
    if (e.keyCode == 38) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-up');
       return false;
    }
    if (e.keyCode == 39) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-right');
       return false;
    }
    if (e.keyCode == 40) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-down');
       return false;
    }
});

$(document).keyup(function(e){
    if (e.keyCode == 37) {
       $('.controller-right').removeClass('pressed-left');
       return false;
    }
    if (e.keyCode == 38) { 
       $('.controller-right').removeClass('pressed-up');
       return false;
    }
    if (e.keyCode == 39) { 
       $('.controller-right').removeClass('pressed-right');
       return false;
    }
    if (e.keyCode == 40) { 
       $('.controller-right').removeClass('pressed-down');
       return false;
    }
});

});

