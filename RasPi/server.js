/*jshint node: true */
var http = require('http'),
	fs = require('fs'),
	path = require('path'),
	io = require('socket.io'),
	serialport = require("serialport");

	
//------------------------------------------------------- Tworzenie serwera
var server = http.createServer(function (req, res) { 
	'use strict';
	var filePath = '.' + req.url,
		contentType = 'text/html',
		extName;

	console.log('request starting...' + filePath);
	if (filePath === './') {
		filePath = './index.html';
	}
	extName = path.extname(filePath);
	switch (extName) {
	case '.js':
		contentType = 'text/javascript';
		break;
	case '.css':
		contentType = 'text/css';
		break;
	}
	
	path.exists(filePath, function (exists) {
		if (exists) {
			fs.readFile(filePath, function (error, content) {
				if (error) {
					res.writeHead(500);
					res.end();
				} else {
					res.writeHead(200, {
						'Content-Type': contentType
					});
					res.end(content, 'utf-8');
				}
			});
		} else {
			res.writeHead(404);
			res.end();
		}
	});
});

//------------------------------------------------------- Obsługa USB

var SerialPort = serialport.SerialPort;
var arduinoSerialPort = '/dev/ttyACM0';
var serialPort = new SerialPort(arduinoSerialPort, {
	parser: serialport.parsers.readline("\n")
});

var socket = io.listen(server);
serialPort.on('open', function() {                
	socket.on('connection', function( client ) {

    	console.log('open - port USB otwarty');

//------------------------------------------------------- Sokety
    	'use strict';
		
		client.on('message', function( message ) {                                        
			console.log( message );
		});
		
		client.on('jazda', function( dane ) {
			console.log( dane );
			//console.log( stanRobota );
			io.sockets.emit( 'testServer', "Test serwer odpowiada");
			io.sockets.emit( 'testJSON', JSON.stringify( daneMapy.test ) );
			serialPort.write( dane );
		});
        
//------------------------------------------------------------------         
		serialPort.on('data', function( dane ) {
			console.log("DANE:" +dane);  
			var daneArduino = JSON.parse( dane );
			
			//console.log('Mapa odebrana od arduino: ' + daneArduino.mapa);
			io.sockets.emit( 'testRobot', daneArduino);
			var linia = '';
			for (var i=0; i<daneArduino.mapa.length; i++) {
				if (i%rozmiarBokuMapy === 0) {
					console.log(linia);
					linia = daneArduino.mapa.charAt(i);
				} else {
					linia = linia + daneArduino.mapa.charAt(i);
				}
			}
		});
		
		serialPort.on('error', function (msg) {
			console.log('Blad przy odbiorze danych z USB Arduino');
		});
    });
});

//------------------------------------------------------- Zmienne

var daneMapy = {
	test: "Test JSON utworzony na serwerze"
};

var stanRobota;

var rozmiarBokuMapy = 10;


//-------------------------------------------------------

server.listen(3030);
console.log('Serwer NODE.JS uruchomiony://localhost:3030/');
