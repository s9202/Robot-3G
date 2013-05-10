/*jshint node: true */
var http = require('http'),
    fs = require('fs'),
    path = require('path'),
    io = require('socket.io'),
	serialport = require('serialport').SerialPort;

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

//------------------------------------------------------- Obs³uga USB

var SerialPort = serialport.SerialPort;
var arduinoSerialPort = '/dev/ttyACM0';	//wskazanie docelowego portu USB w Raspberym 

var serialPort = new serialport(arduinoSerialPort,
{
	//parser: serialport.parsers.readline('\n'),
	//domyœlny parser emituje "data". Wykorzystywane do odbioru danych z serialportu od arduino
	baudrate: 9600 //prêdkoœæ przesy³ania bitów przez serialport
	}
);


serialPort.on('open', function() {								// Wy³¹czone na czas braku portu
		console.log('open - port USB otwarty');
	
		
		
	
//------------------------------------------------------- Sokety

var socket = io.listen(server);
	
socket.on('connection', function( client ) {
    'use strict';
	console.log('port zaraz sie otworzy');
	
	
		client.on('message', function( message ) {					//obecnie nieu¿ywane
			console.log( message );
		});
		
		client.on('jazda', function( dane ) {

			console.log( dane );
			console.log( stanRobota );
			client.emit( 'testServer', "Test serwer odpowiada");
			client.emit( 'testJSON', JSON.stringify( daneMapy.test ) );
			serialPort.write( dane  ); 			// Wy³¹czone na czas braku portu

		});
		
//------------------------------------------------------------------ Wy³¹czone na czas braku portu		
		//serialport.on( "data", function( dane ) {
		//	stanRobota = JSON.parse( dane );		// przetwarzanie odebranego JSONa
			//client.emit( 'testRobot', stanRobota );
			//console.log('odebrano dane z USB Arduino');
		//});
		
		serialPort.on("error", function (msg) {
			console.log('Blad przy odbiorze danych z USB Arduino');
		});
		
	});

});

//------------------------------------------------------- Zmienne

var daneMapy = {
	test: "Test JSON utworzony na serwerze"
};

var stanRobota = { komunikat: "diodaJSON" };


//-------------------------------------------------------

server.listen(3030);
console.log('Serwer NODE.JS uruchomiony://localhost:3030/');
