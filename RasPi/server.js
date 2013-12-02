/*jshint node: true */
var http = require('http'),
    fs = require('fs'),
    path = require('path'),
    io = require('socket.io'),
    serialport = require("serialport");
    
    
/*    
    dgram = require('dgram')
    child = require('child_process');

// -------------------------------------------------------server testowy - kamera
var server2 = http.createServer(function (req, res) { 
 date = new Date();
 
  res.writeHead(200, {
    'Date':date.toUTCString(),
    'Connection':'close',
    'Cache-Control':'private',
    'Content-Type':'video/webm',
    'Server':'CustomStreamer/0.0.1',
    });
 
  var udpServer = dgram.createSocket('udp6');
    
  udpServer.on('message', function(msg, info) {
    res.write(msg);
  });
  
  udpServer.on('close', function() {
    res.end();
  });
 
  udpServer.on('error', function(error) {
    res.end();
  });
  
  udpServer.on('listening', function() {
    var cmd = 'gst-launch-0.10';
    var address = udpServer.address();
    var args =  ['v4l2src', 'device=/dev/video0', 
                 //'!', 'video/x-raw-rgb','width=320','height=240','framerate=5/1', - to dziala jak jest zainstalowany codec
                '!' ,'ffmpegcolorspace',
		        '!', 'vp8enc', 'speed=4',
                '!', 'm.', 'webmmux', 'name=m', 'streamable=true',
                 '!', 'udpsink', 'clients=localhost:'+address.port];
    var options = null;

    var gstCam = child.spawn(cmd, args, options);
 
  
 
res.connection.on('close', function() {
      gstCam.kill();
      udpServer.close();
    });
  });
  
  udpServer.bind(0, 'localhost');
});
server2.listen(3031);	
	
*/	
	
	
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
var arduinoSerialPort = '/dev/ttyACM0';
var serialPort = new SerialPort(arduinoSerialPort, {
parser: serialport.parsers.readline("\n")
});

var socket = io.listen(server);
serialPort.on('open', function() {                
socket.on('connection', function( client ) {
                                                        // Wy³¹czone na czas braku portu
                console.log('open - port USB otwarty');
        
                
                
        
//------------------------------------------------------- Sokety


    'use strict';
        
        
                client.on('message', function( message ) {                                        
                        console.log( message );
                });
                
                client.on('jazda', function( dane ) {

                        console.log( dane );
                        //console.log( stanRobota );
                        client.emit( 'testServer', "Test serwer odpowiada");
                        client.emit( 'testJSON', JSON.stringify( daneMapy.test ) );
                        serialPort.write( dane  );                         

                });
                
//------------------------------------------------------------------         
               serialPort.on('data', function( dane ) {
                        console.log("DANE:" +dane);  
                        var daneArduino = JSON.parse( dane );
			
//                        console.log('Mapa odebrana od arduino: ' + daneArduino.mapa);
                        client.emit( 'testRobot', daneArduino);
                        var linia = '';
                        for (var i=0; i<daneArduino.mapa.length; i++) {
                        
                                if (i%10 === 0) {
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


//-------------------------------------------------------

server.listen(3030);
console.log('Serwer NODE.JS uruchomiony://localhost:3030/');
