/*jshint node: true, browser: true, jquery: true */
/*global io: false */
$(document).ready(function () {
    'use strict';
    var socket = io.connect();

//--------------------------------------------------------------- obs³uga odpowiedzi serwera
    socket.on('testServer', function( dane ){
        console.log( dane );
		$("#d1").css( { "background-color": "green" } );
		$("#d3").css( { "background-color": "white" } );
    });
	
	socket.on('testJSON', function( dane ){
        console.log( JSON.parse( dane ) );
		$("#d2").css( { "background-color": "green" } );
    });
	
	socket.on('testRobot', function( dane ) {
		console.log( dane );
		if( dane.komunikat === "DIODA SWIECI" ) {
			$("#d3").css( { "background-color": "green" } );
		}
		else if( dane.komunikat === "dioda JSON" ) {
			$("#d3").css( { "background-color": "white" } );
		}
		else {
			$("#d3").css( { "background-color": "red" } );
		}
	});

	
//------------------------------------------------------------------



//------------------------------------------------------------------- Mysz
	
	$("#w").on( "mousedown", function() {
		$(this).css( { "background-color": "blue" } );
//		socket.emit('jazda', 'przodON');
		socket.emit('jazda', '1');
		console.log('wcisniete W');
     })
	 .on( "mouseup", function() {
		$(this).css( { "background-color": "red" } );
//		socket.emit('jazda', 'przodOFF');
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
	});
	
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
		}
	});
	
//---------------------------------------------------------------------
	
});
