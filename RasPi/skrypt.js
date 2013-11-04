/*jshint node: true, browser: true, jquery: true */
/*global io: false */
$(document).ready(function () {
    'use strict';
    var socket = io.connect();

        var rozmiar = 700;
        var robotX = 301, robotY = 301;
        var robotHeadX = robotX+10, robotHeadY = robotY-3;

        var robotLIRX = robotX-3, robotLIRY = robotY+27;
        var robotRIRX = robotX+41, robotRIRY = robotY+12;
        var robotHIRX = robotX+13, robotHIRY = robotY-3;
        



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
        var robot = paper.path("M "+ robotX +" "+ robotY +" l 0 38 l 38 0 l 0 -38 z"); 
        robot.attr({fill: '#B30000', stroke: 'green', 'stroke-width': 2}); 

        //var robotHead = paper.path("M "+ robotHeadX+" "+ robotHeadY +"  l 5 0 l 0 -3.5 l 0 3.5 l 10 0 l 0 -3.5 l 0 3.5 l 5 0");
        //robotHead.attr({stroke: 'green', 'stroke-width': 3});
        
        var robotHIR = paper.path("M "+ robotHIRX+" "+ robotHIRY +   "  l 4 0 l 0 -3.5 l 0 3.5 l 6 0 l 0 -3.5 l 0 3.5 l 4 0");
        robotHIR.attr({stroke: 'green', 'stroke-width': 3});

        var robotLIR = paper.path("M "+ robotLIRX+" "+ robotLIRY+"l 0 -4 l -3.5 0 l  3.5 0 l 0 -6 l -3.5 0 l 3.5 0 l 0 -4");
        robotLIR.attr({stroke: 'green', 'stroke-width': 3});
        
        var robotRIR = paper.path("M "+ robotRIRX+" "+ robotRIRY+"l 0 4 l 3.5 0 l  -3.5 0 l 0 6 l 3.5 0 l -3.5 0 l 0 4");
        robotRIR.attr({stroke: 'green', 'stroke-width': 3});        
}


//--------------------------------------------------------------- obs³uga odpowiedzi serwera
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
                if(dane) {
                        $("#d3").css( { "background-color": "green" } );
                }
                else {
                        $("#d3").css( { "background-color": "red" } );
                }
                
                
                
                $("#dane").empty();//czyszczenie okienka komunikatow
                for (var i=0; i<dane.length; i++) {//tablica w okienku komunikatow                    
					if (i%10 === 0) {
						if(i>1) $("#dane").append("<br>");
                                $("#dane").append(dane.charAt(i));
                        } else {
                $("#dane").append(dane.charAt(i));
            }
        }
                
                
                
                drawBlock();
                draw();
                
        });

        
//------------------------------------------------------------------



//------------------------------------------------------------------- Mysz
        
	$("#w")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'W');
			console.log('wcisniete W');
		 })
		.on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
			//socket.emit('jazda', '3');
			console.log('niewcisniete W');
		});
	
	$("#a")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'A');
			console.log('wcisniete A');
		 })
		.on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
			//socket.emit('jazda', '3');
			console.log('niewcisniete A');
		});//robotHead.rotate(50,robotHeadX+20,robotHeadY);
	
	$("#s")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'S');
			console.log('wcisniete S');
		 })
		.on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
			//socket.emit('jazda', '3');
			console.log('niewcisniete S');
		});
	
	$("#d")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'D');
			console.log('wcisniete D');
		 })
		 .on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
			//socket.emit('jazda', '3');
			console.log('niewcisniete D');
		});
		
	$("#q")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'q');
			robotHead.rotate(-5,robotHeadX+20,robotHeadY);
			console.log('wcisniete Q');
		 })
		 .on( "mouseup", function() {
			$(this).css( { "background-color": "green" } );
			socket.emit('jazda', '4');
			console.log('niewcisniete Q');
		});
	$("#e")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'e');
			robotHead.rotate(5,robotHeadX+20,robotHeadY);
			console.log('wcisniete E');
		 })
		.on( "mouseup", function() {
			$(this).css( { "background-color": "green" } );
			socket.emit('jazda', '4');
			console.log('niewcisniete R');
	});
	


	$("#tryb1")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "orange" } );
			$("#tryb2").css( { "background-color": "yellow" } );
			socket.emit('jazda', '0');
			console.log('tryb servo');
		})
	
	$("#tryb2")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "orange" } );
			$("#tryb1").css( { "background-color": "yellow" } );
			socket.emit('jazda', '1');
			console.log('tryb sonic');
		})
	
	$("#skan")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "yellow" } );
			socket.emit('jazda', '5');
			console.log('Skan manualny');
		})
		.on( "mouseup", function() {
			$(this).css( { "background-color": "teal" } );
		});
//-------------------------------------------------------- Klawiatura
        
   $(document).on( "keydown", function(event) {
                switch(event.keyCode) {
                case 87:
                        $("#w").css( { "background-color": "blue" } );
                        socket.emit('jazda', 'w');
                        console.log('wcisniete klawisz W');
                        break;
                case 65:
                        $("#a").css( { "background-color": "blue" } );
                        socket.emit('jazda', 'a');
                        console.log('wcisniete klawisz A');
                        break;
                case 83:
                        $("#s").css( { "background-color": "blue" } );
                        socket.emit('jazda', 's');
                        console.log('wcisniete klawisz S');
                        break;
                case 68:
                        $("#d").css( { "background-color": "blue" } );
                        socket.emit('jazda', 'd');
                        console.log('wcisniete klawisz D');
                        break;

                case 81:
                        $("#q").css( { "background-color": "blue" } );
                        socket.emit('jazda', 'q');
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
                        socket.emit('jazda', '3');
                        console.log('niewcisniete klawisz W');
                        break;
                case 65:
                        $("#a").css( { "background-color": "red" } );
                        socket.emit('jazda', '3');
                        console.log('niewcisniete klawisz A');
                        break;
                case 83:
                        $("#s").css( { "background-color": "red" } );
                        socket.emit('jazda', '3');
                        console.log('niewcisniete klawisz S');
                        break;
                case 68:
                        $("#d").css( { "background-color": "red" } );
                        socket.emit('jazda', '3');
                        console.log('niewcisniete klawisz D');
                        break;
                case 81:
                        $("#q").css( { "background-color": "green" } );
                        socket.emit('jazda', '4');
                        break;
                
                case 69:
                        $("#e").css( { "background-color": "green" } );
                        socket.emit('jazda', '4');
                        break;

                }

        });
        
//---------------------------------------------------------------------
        
function drawBlock(){        
        paper.clear();        
        var block = paper.rect(0, 0, 40, 40, 2);
        block.attr("fill", "blue");
        block.attr("stroke", "#fff");
        
}        
});
