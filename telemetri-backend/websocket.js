
var WebSocket = require('ws');
var wsClient = new WebSocket('ws://localhost:2500');
const SerialPort = require('serialport');
const parsers = SerialPort.parsers;
var GPS = require('gps');
var gps = new GPS;
var net = require('net');
var file = '/dev/ttyUSB1';      //067b prolific


var admin = require("firebase-admin");
var serviceAccount = require("/home/pi/telemetri/car-telemetry-firebase-adminsdk.json");

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://car-telemetry.firebaseio.com"
});

var database = admin.database();

setTimeout(function(){

    var dataLoc = database.ref("/location");

    setInterval(function(){
        dataLoc.set({
            lat: lat,
            long: long
        })

    }, 1000);
}, 3000);

var rpio = require('rpio');

var pin1 = 3;
var pin2 = 5;

var pin1 = 0, pin2 = 7;
wpi.pinMode(pin1, wpi.INPUT);
wpi.pinMode(pin2, wpi.INPUT);

rpio.open(pin1, rpio.INPUT, rpio.PULL_UP);
rpio.open(pin2, rpio.INPUT, rpio.PULL_UP);

var client = new net.Socket();

client.connect(1337, 'localhost', function() {
	console.log('Connected');
	//client.write('Hello, server! Love, Client.');
});

client.on('data', function(p_data)
{
    console.log("Val: " + p_data);

    inStr = p_data;
});

const parser = new parsers.Readline({
    delimiter: '\r\n'
});

var long = 0, lat = 0;


gps.on('data', function(data) {
  //console.log(data);
  long = data["lon"];
  lat = data["lat"];
});

parser.on('data', function(data) {
  gps.update(data);
});

var portName = "/dev/ttyUSB0";
SerialPort.list(function (err, ports) {
  ports.forEach(function(port) {
	if(port.vendorId == "067b")			//ft232
	{
		portName = port.comName;
	}
	//console.log(port);
  });
});

setTimeout(function(){
    const port = new SerialPort(portName, {
        baudRate: 9600
    });

    port.on('open',function()
    {
        console.log('SerialPort is open..');
    });

    port.pipe(parser);
}, 1000);

var sendInterval;

wsClient.on('open', function() {
	console.log('server connected');
    connected = true;
    	
    sendInterval = setInterval(function(){

		var xSpeed = Math.floor((Math.random() * 100) + 1);
		var xLat = 0;
		var xLon = 0;

        if(lat !== "undefined" && lat != null)
            xLat = lat;

        if(long !== "undefined" && long != null)
            xLon = long;

		var getString = "#" + "|" + xLat.toString() 
                            + "|" + xLon.toString()
                            + "|" + inStr;

        
        //console.log(getString);

        try{
            wsClient.send(getString);
        }catch(err)
        {
            console.log(err);

            
        }

        if(rpio.read(pin1))
        {
            wsClient.send("+|1|");
        }

        if(rpio.read(pin2))
        {
            wsClient.send("-|1|");
        }
	}, 100);

    /*
    socket.on('close', function(){
        console.log("closed");
        clearInterval(sendInterval);
    })*/
});