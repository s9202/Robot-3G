/*jshint node: true, browser: true, jquery: true */
/*global io: false */
$(document).ready(function () {
    'use strict';
    var socket = io.connect();

	var rozmiar = 700;
	var robotX = 301, robotY = 301;
	var robotHeadX = 301+20, robotHeadY = 301-7;
	var paper = Raphael(mapa, rozmiar, rozmiar);
function draw(){	
	var liniePoziom = paper.path("M 0 0 l "+ rozmiar +" 0 ");
	var liniePion = paper.path("M 0 0 l 0 "+ rozmiar);  
	for(var i = 0; i < 1000; i+=20) {
		liniePoziom = paper.path("M 0 "+i+" l "+rozmiar+" 0 ");
		liniePion = paper.path("M"+i+" 0 l 0 "+rozmiar);  
		liniePoziom.attr({stroke: 'white', 'stroke-width': 1});
		liniePion.attr({stroke: 'white', 'stroke-width': 1});		
	}
	var robot = paper.path("M "+ robotX +" "+ robotY +" l 0 77 l 77 0 l 0 -77 z"); 
	robot.attr({fill: '#B30000', stroke: 'green', 'stroke-width': 3}); 

	var robotHead = paper.path("M "+ robotHeadX+" "+ robotHeadY +"  l 10 0 l 0 -7 l 0 7 l 20 0 l 0 -7 l 0 7 l 10 0");
	robotHead.attr({stroke: 'green', 'stroke-width': 7});  
}


//--------------------------------------------------------------- obs�uga odpowiedzi serwera
    socket.on('testServer', function( dane ){
        console.log( dane );
		$("#d1").css( { "background-color": "green" } );
    });
	
	socket.on('testJSON', function( dane ){
        console.log( JSON.parse( dane ) );
		$("#d2").css( { "background-color": "green" } );
    });
	
	socket.on('testRobot', function( dane ) {
		
		console.log('from skrypt'+ dane );
		if( dane== 1 ) {
			$("#d3").css( { "background-color": "green" } );
		}
		else {
			$("#d3").css( { "background-color": "red" } );
		}
		paper.clear();	
		var circle = paper.circle(robotHeadX+40, robotHeadY-dane, 8);
		circle.attr("fill", "blue");
		circle.attr("stroke", "#fff");
		draw();
	});

	
//------------------------------------------------------------------



//------------------------------------------------------------------- Mysz
	
	$("#w").on( "mousedown", function() {
		$(this).css( { "background-color": "blue" } );
		socket.emit('jazda', '1');
		console.log('wcisniete W');
     })
	 .on( "mouseup", function() {
		$(this).css( { "background-color": "red" } );
		socket.emit('jazda', '0');
		console.log('niewcisniete W');
	});
	
	$("#a").on( "mousedown", function() {
		$(this).css( { "background-color": "blue" } );
        	socket.emit('jazda', '3');
		console.log('wcisniete A');
     })
	 .on( "mouseup", function() {
		$(this).css( { "background-color": "red" } );
		socket.emit('jazda', '2');
		console.log('niewcisniete A');
	});//robotHead.rotate(50,robotHeadX+20,robotHeadY);
	
	$("#s").on( "mousedown", function() {
		$(this).css( { "background-color": "blue" } );
        	socket.emit('jazda', '5');
		console.log('wcisniete S');
     })
	 .on( "mouseup", function() {
		$(this).css( { "background-color": "red" } );
		socket.emit('jazda', '4');
		console.log('niewcisniete S');
	});
	
	$("#d").on( "mousedown", function() {
		$(this).css( { "background-color": "blue" } );
        	socket.emit('jazda', '7');
		console.log('wcisniete D');
     })
	 .on( "mouseup", function() {
		$(this).css( { "background-color": "red" } );
		socket.emit('jazda', '6');
		console.log('niewcisniete D');
	});
	$("#q").on( "mousedown", function() {
		$(this).css( { "background-color": "blue" } );
		socket.emit('jazda', 'l');
        	robotHead.rotate(-5,robotHeadX+20,robotHeadY);
		console.log('wcisniete Q');
     })
	 .on( "mouseup", function() {
		$(this).css( { "background-color": "green" } );
		socket.emit('jazda', 'a');
		console.log('niewcisniete Q');
	});
	$("#e").on( "mousedown", function() {
		$(this).css( { "background-color": "blue" } );
		socket.emit('jazda', 'e');
        	robotHead.rotate(5,robotHeadX+20,robotHeadY);
		console.log('wcisniete E');
     })
	 .on( "mouseup", function() {
		$(this).css( { "background-color": "green" } );
		socket.emit('jazda', 'b');
		console.log('niewcisniete R');
	});
	


	$("#tryb1").on( "mousedown", function() {
		$(this).css( { "background-color": "orange" } );
		$("#tryb2").css( { "background-color": "yellow" } );
		socket.emit('jazda', 'o');
		console.log('tryb servo');
      })
	$("#tryb2").on( "mousedown", function() {
		$(this).css( { "background-color": "orange" } );
		$("#tryb1").css( { "background-color": "yellow" } );
		socket.emit('jazda', 'c');
		console.log('tryb sonic');
     })
//-------------------------------------------------------- Klawiatura
	
	$(document).on( "keydown", function(event) {
		switch(event.keyCode) {
		case 87:
			$("#w").css( { "background-color": "blue" } );
			socket.emit('jazda', '1');
			console.log('wcisniete klawisz W');
			break;
		case 65:
			$("#a").css( { "background-color": "blue" } );
			socket.emit('jazda', '3');
			console.log('wcisniete klawisz A');
			break;
		case 83:
			$("#s").css( { "background-color": "blue" } );
			socket.emit('jazda', '5');
			console.log('wcisniete klawisz S');
			break;
		case 68:
			$("#d").css( { "background-color": "blue" } );
			socket.emit('jazda', '7');
			console.log('wcisniete klawisz D');
			break;

		case 81:
			$("#q").css( { "background-color": "blue" } );
			socket.emit('jazda', 'l');
        		robotHead.rotate(-5,robotHeadX+20,robotHeadY);
			console.log('wcisniete Q');
			break;
		
		case 69:
			$("#e").css( { "background-color": "blue" } );
			socket.emit('jazda', 'e');
        		robotHead.rotate(5,robotHeadX+20,robotHeadY);
			console.log('wcisniete E');
			break;
		}
		
     })
	 .on( "keyup", function(event) {
		switch(event.keyCode) {
		case 87:
			$("#w").css( { "background-color": "red" } );
			socket.emit('jazda', '0');
			console.log('niewcisniete klawisz W');
			break;
		case 65:
			$("#a").css( { "background-color": "red" } );
			socket.emit('jazda', '2');
			console.log('niewcisniete klawisz A');
			break;
		case 83:
			$("#s").css( { "background-color": "red" } );
			socket.emit('jazda', '4');
			console.log('niewcisniete klawisz S');
			break;
		case 68:
			$("#d").css( { "background-color": "red" } );
			socket.emit('jazda', '6');
			console.log('niewcisniete klawisz D');
			break;
		case 81:
			$("#q").css( { "background-color": "green" } );
			socket.emit('jazda', '10');
			break;
		
		case 69:
			$("#e").css( { "background-color": "green" } );
			socket.emit('jazda', '14');
			break;

		}

	});
	
//---------------------------------------------------------------------
	
});
