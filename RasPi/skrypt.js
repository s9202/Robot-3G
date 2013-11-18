/*jshint node: true, browser: true, jquery: true */
/*global io: false */
$(document).ready(function () {
    'use strict';
var socket = io.connect();

var counter = 0;

//--------------------------------------
var rozmiar = 700;
var paper = Raphael(mapa, rozmiar, rozmiar);
var robot;
var robotHIR, robotLIR, robotRIR; 
var timeForward = 1170;
var timeRotate = 500;


function drawLines(){        
        var liniePoziom = paper.path("M 0 0 l "+ rozmiar +" 0 ");
        var liniePion = paper.path("M 0 0 l 0 "+ rozmiar);  
        for(var i = 0; i < 1000; i+=20) {
                liniePoziom = paper.path("M 0 "+i+" l "+rozmiar+" 0 ");
                liniePion = paper.path("M"+i+" 0 l 0 "+rozmiar);  
                liniePoziom.attr({stroke: 'white', 'stroke-width': 1});
                liniePion.attr({stroke: 'white', 'stroke-width': 1});                
        }      
}

function drawBlock(nrX, nrY){           	       	
	var block = paper.rect(nrX*40, nrY*40, 40, 40, 4);
        block.attr("fill", "blue");
        block.attr("stroke", "#fff");
        
}

function drawRobot(nrX, nrY, obrot){  
	
	var robotX = nrX*40;
	var robotY = nrY*40;
        var robotLIRX = robotX-3+5, robotLIRY = robotY+33;
        var robotRIRX = robotX+41-5, robotRIRY = robotY+19;
        var robotHIRX = robotX+12, robotHIRY = robotY-3+5;
        
	

	robot = paper.rect(robotX+4, robotY+4, 30, 34, 5);       
	robot.attr({fill: '#B30000', stroke: 'green', 'stroke-width': 2}); 
        
        robotHIR = paper.path("M "+ robotHIRX+" "+ robotHIRY +   "  l 4 0 l 0 -3.5 l 0 3.5 l 6 0 l 0 -3.5 l 0 3.5 l 4 0");
        robotHIR.attr({stroke: 'green', 'stroke-width': 3});

        robotLIR = paper.path("M "+ robotLIRX+" "+ robotLIRY+"l 0 -4 l -3.5 0 l  3.5 0 l 0 -6 l -3.5 0 l 3.5 0 l 0 -4");
        robotLIR.attr({stroke: 'green', 'stroke-width': 3});
        
        robotRIR = paper.path("M "+ robotRIRX+" "+ robotRIRY+"l 0 4 l 3.5 0 l  -3.5 0 l 0 6 l 3.5 0 l -3.5 0 l 0 4");
        robotRIR.attr({stroke: 'green', 'stroke-width': 3});  
        
	if(obrot == 4){ //obrot w lewo
		robot.transform("t2,-1r-90");
		robotHIR.transform("t-18,20r-90");
		robotLIR.transform("t26,12r-90");
		robotRIR.transform("t-12,-25r-90");
	}
        
	else if(obrot == 2){ //obrot 180
		robot.transform("t1,-2rr-180");
		robotHIR.transform("t1,39r-180");
		robotLIR.transform("t38,-11r-180");
		robotRIR.transform("t-36,-11r-180");
	}
	else if(obrot == 6){ //obrot w prawo
		robot.transform("t0,-1r90");
		robotHIR.transform("t20,20r90");
		robotLIR.transform("t13,-24r90");
		robotRIR.transform("t-25,12r90");
	}
}
function removeRobot(){  
	robot.remove();
	robotHIR.remove();
	robotLIR.remove();
	robotRIR.remove();      
}

function moveRobot(){

	
	robot.animate({transform: "t0,-40"}, timeForward);
	robotHIR.animate({transform: "t0,-40"}, timeForward);
	robotLIR.animate({transform: "t0,-40"}, timeForward);
	robotRIR.animate({transform: "t0,-40"}, timeForward);
	

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
                var daneArduinoSerwera = dane;
                var daneMapy = daneArduinoSerwera.mapa;
                console.log('from skrypt'+ daneMapy );
                if (daneMapy) {
                        $("#d3").css( { "background-color": "green" } );
                }
                else {
                        $("#d3").css( { "background-color": "red" } );
                }
                
		//tymczasowe czyszczenie koncowych znakow
		//daneMapy = setCharAt(dane, 100, 't');
		//daneMapy = setCharAt(dane, 101, 't');
		//daneMapy = setCharAt(dane, 102, 't');
		 
	
                $("#dane").empty();//czyszczenie okienka komunikatow
                for (var i=0; i<daneMapy.length; i++) {//tablica w okienku komunikatow                    
			if (i%10 === 0) {
				
				$("#dane").append("<br>");
			}
                	$("#dane").append(daneMapy.charAt(i));
        	}
 


		//drawing
		drawLines();               
                var drawX=0, drawY=-1;
		for(var i=0; i<daneMapy.length; i++){			
			if (i%10 === 0) {
				drawX=0;
				drawY++;
                        } else {
 				drawX++;
            		}
			if( (daneMapy.charAt(i)=== "x" ) || (daneMapy.charAt(i)==="P") || (daneMapy.charAt(i)===  "L") ) drawBlock(drawX, drawY);
			if( (daneMapy.charAt(i)==="8") || (daneMapy.charAt(i)==="4") || (daneMapy.charAt(i)==="2") || (daneMapy.charAt(i)==="6") ){
				if(counter>0) removeRobot();		
				
				drawRobot(drawX, drawY, daneMapy.charAt(i));
				//moveRobot();
			}
		}              
               
		
		
        counter++;        
        });//socket.on 'testRobot'

        
//------------------------------------------------------------------



//------------------------------------------------------------------- Mysz
        
	$("#w")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'W');
			//socket.emit('jazda', '5');
			console.log('wcisniete W');
		 })
		.on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
			console.log('niewcisniete W');
			
		});
	
	$("#a")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'A');
			//socket.emit('jazda', '5');
			console.log('wcisniete A');
		 })
		.on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
			console.log('niewcisniete A');
		});
	
	$("#s")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'S');
			//socket.emit('jazda', '5');
			console.log('wcisniete S');
		 })
		.on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
			console.log('niewcisniete S');
		});
	
	$("#d")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "blue" } );
			socket.emit('jazda', 'D');
			//socket.emit('jazda', '5');
			console.log('wcisniete D');
		 })
		 .on( "mouseup", function() {
			$(this).css( { "background-color": "red" } );
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
			console.log('tryb manual');
		})
	
	$("#tryb2")
		.on( "mousedown", function() {
			$(this).css( { "background-color": "orange" } );
			$("#tryb1").css( { "background-color": "yellow" } );
			socket.emit('jazda', '1');
			console.log('tryb auto');
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
        
//---------------------------------------------------------------------funkcje pomocnicze
function setCharAt(str, index, chr){// funkcja do zamiany znaku stringu
	if(index > str.length-1) return str;
	return str.substr(0,index) + chr + str.substr(index+1);
}        




        
});
