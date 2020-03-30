var net=require("net");
const SerialPort = require('serialport');
const parsers = SerialPort.parsers;
const parser = new parsers.Readline({
  delimiter: '\r\n'
});

var admin = require("firebase-admin");
var serviceAccount = require("/home/pi/telemetri/car-telemetry-firebase-adminsdk.json");

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://car-telemetry.firebaseio.com"
});

var veri;
var all = {


	valueCabin    : [],
	valueMotorRight  : [],
	valueMotorLeft : [],
	valueHor     : [],
	valueBattery : [],
	valueLight   : [],

};

var portName = "/dev/ttyUSB0";
SerialPort.list(function (err, ports) {
  ports.forEach(function(port) {
	if(port.vendorId == "0403")			//ft232
	{
		portName = port.comName;
	}
	//console.log(port);
  });
});

setTimeout(function(){ 

const port = new SerialPort(portName, {
  baudRate: 115200
});

port.pipe(parser);

port.on('open',function()
{
	console.log('SerialPort is open..');
});

port.on('error', function(err) {
  console.log('Error: ', err.message);
});


parser.on('data',function (val) {
   veri = val.split("|");

   //console.log(val);

  if(veri[0] == "#")
  {
	veri = veri.map(Number);

	if(veri[1] == 0x43)				//batarya 1
	{
		all["valueBattery"]["cell0"]=veri[2];
		all["valueBattery"]["cell1"]=veri[3];
		all["valueBattery"]["cell2"]=veri[4];
		all["valueBattery"]["cell3"]=veri[5];
		all["valueBattery"]["cell4"]=veri[6];
		all["valueBattery"]["cell5"]=veri[7];
		all["valueBattery"]["cell6"]=veri[8];
		all["valueBattery"]["cell7"]=veri[9];
	}

	if(veri[10] == 0x44)			//batarya 2
	{
		all["valueBattery"]["cell8"]=veri[11];
		all["valueBattery"]["cell9"]=veri[12];
		all["valueBattery"]["cell10"]=veri[13];
		all["valueBattery"]["cell11"]=veri[14];
		all["valueBattery"]["cell12"]=veri[15];
		all["valueBattery"]["cell13"]=veri[16];
		all["valueBattery"]["cell14"]=veri[17];
		all["valueBattery"]["temp1"]=veri[18];
	}

	if(veri[19] == 0x45)			//batarya 3
	{
		all["valueBattery"]["temp2"]=veri[19];
		all["valueBattery" ]["batteryTemp"] =veri[20];
		all["valueBattery"]["current"]=veri[21];
	}

	if(veri[28] == 0x40)			//sol motor
	{
		all["valueMotorLeft" ]["motorLeftTemp"] =veri[29];
		all["valueMotorLeft"]["LeftMotorSpeed"]   =veri[30];
		all["valueMotorLeft"]["MotorDriverLInput"]  =veri[31];
		all["valueMotorLeft"]["MotorDriverLOutput"] =veri[32];
		all["valueMotorLeft"]["MotorDriverLVINPUT"]  =veri[33];
		all["valueMotorLeft"]["MotorDriverLVOUTPUT"] =veri[34];
	}

	if(veri[37] == 0x41)			//sağ motor
	{
		all["valueMotorRight" ]["motorRightTemp"] =veri[38];
		all["valueMotorRight"]["RightMotorSpeed"] =veri[39];
		all["valueMotorRight"]["MotorDriverRInput"]  =veri[40];
		all["valueMotorRight"]["MotorDriverROutput"] =veri[41];
		all["valueMotorRight"]["MotorDriverRVINPUT"]  =veri[42];
		all["valueMotorRight"]["MotorDriverRVOUTPUT"] =veri[43];
	}

	if(veri[46] == 0x42)			//kabin
	{
		all["valueCabin" ]["inTemp"] =veri[47];
		all["valueCabin" ]["outTemp"] =veri[48];
		//all["valueCabin"]["leftDoor"]  = veri[49];
		//all["valueCabin"]["sagKapı"]  = veri[50];
		//all["valueCabin"]["flasher"]  = veri[51];
		//all["valueCabin"]["topLight"]    =veri[52];
		all["valueCabin"]["h2Sens"]   =veri[53];
	}

	if(veri[55] == 0x46)			//horizon
	{
		all["valueHor"]["status"]=veri[56];
		all["valueHor"]["ambientTemp"]=veri[57];
		all["valueHor"]["stackVoltage"]=veri[58];
		all["valueHor"]["H2Voltage"]=veri[59];
		all["valueHor"]["stackTemp"]=veri[60];
		all["valueHor"]["current"]=veri[61];
		all["valueHor"]["batteryVoltage"]=veri[62];
		all["valueHor" ]["h2Temp"] =veri[63];
	}
	

    if(connected)
    {
		var tArray = [], cArray = [], sSend = "";

		tArray.push({"ID" : "Batarya 1" ,"val" : all["valueBattery"]["temp1"]});
		tArray.push({"ID" : "Batarya 2", "val" : all["valueBattery"]["temp2"]});
		tArray.push({"ID" : "Horizon Stack", "val" : all["valueHor"]["stackTemp"]});
		tArray.push({"ID" : "Kabin İç", "val" : all["valueCabin" ]["inTemp"]});
		tArray.push({"ID" : "Kabin Dış", "val" : all["valueCabin" ]["outTemp"]});
		tArray.push({"ID" : "Sağ Motor", "val" : all["valueMotorRight" ]["motorRightTemp"]});
		tArray.push({"ID" : "Sol Motor", "val" : all["valueMotorLeft" ]["motorLeftTemp"]});
		tArray.push({"ID" : "Horizon Amb", "val" : all["valueHor"]["ambientTemp"]});

		tArray.sort(function(a, b){return a.val - b.val});

		cArray.push({"ID" : "Sol Motor G" ,"val" : all["valueMotorLeft"]["MotorDriverLInput"]});
		cArray.push({"ID" : "Sol Motor Ç", "val" : all["valueMotorLeft"]["MotorDriverLOutput"]});
		cArray.push({"ID" : "Sağ Motor G", "val" : all["valueMotorRight"]["MotorDriverRInput"]});
		cArray.push({"ID" : "Sol Motor Ç", "val" : all["valueMotorRight"]["MotorDriverROutput"]});
		cArray.push({"ID" : "Horizon", "val" : all["valueHor"]["current"]});
		cArray.push({"ID" : "Batarya", "val" : all["valueBattery"]["current"]});
		
		cArray.sort(function(a, b){return a.val - b.val});
    	cArray.reverse();tArray.reverse();

		var tSpeed = (parseInt(all["valueMotorLeft"]["LeftMotorSpeed"]) + parseInt(all["valueMotorRight"]["RightMotorSpeed"])) / 2.0;
		
		sSend = String(all["valueMotorLeft"]["LeftMotorSpeed"]) + 
				  "|" + String(all["valueMotorRight"]["RightMotorSpeed"]) 
				+ "|" + String(tSpeed)  
				+ "|" + String(tArray[0]["ID"]) + "|" + String(tArray[0]["val"])
				+ "|" + String(cArray[0]["ID"]) + "|" + String(cArray[0]["val"])
				;

		for(var i = 0; i < tArray.length; i++)
		{
			sSend += "|" + String(tArray[i]["ID"]) + "|" + String(tArray[i]["val"]);
		}

		for(var i = 0; i < cArray.length; i++)
		{
			sSend += "|" + String(cArray[i]["ID"]) + "|" + String(cArray[i]["val"]);
		}

/*
		sSend += "|Sol Motor Ç" + String(all["valueMotorLeft"]["MotorDriver2VOUTPUT"]);
		sSend += "|" + String(all["valueMotorLeft"]["MotorDriver2VOUTPUT"]);
		sSend += "|" + String(all["valueMotorRight"]["MotorDriver2VINPUT"]);
		sSend += "|" + String(all["valueMotorRight"]["MotorDriver2VOUTPUT"]);
		sSend += "|" + String(all["valueHor"]["batteryVoltage"]);*/


		//console.log(sSend);
        aSocket.write(sSend);
    }

  }else if(veri[0] == "&")
  {
	  var sSend = "&|" + String(veri[1]) + "|" + String(parseInt(veri[2]));

	  veri = veri.map(Number);

	  //console.log(veri);

	  if(connected)
	  {
	  	aSocket.write(sSend);
	  }

	  switch(parseInt(veri[1]))
	  {
			case SHORT_LIGHT_ANS:
		  		dataRefLight.update({shortL: veri[2]});
				break;
			case LONG_LIGHT_ANS:
				dataRefLight.update({longL: veri[2]});
				break;
			case LEFT_SIGNAL_ANS:
				dataRefLight.update({leftSignal: veri[2]});
				break;
			case RIGHT_SIGNAL_ANS:
				dataRefLight.update({rightSignal: veri[2]});
				break;
			case SIGNAL_ANS:
				dataRefLight.update({signal: veri[2]});
				break;
			case NEO_PIXEL1_ANS:
				dataRefLight.update({neoPixel1: veri[2]});
				break;
			case NEO_PIXEL2_ANS:
				dataRefLight.update({neoPixel2: veri[2]});
				break;	
			case WIPER_ANS:
				dataRefLight.update({wiper: veri[2]});
				break;
			case TOP_LIGHT_ANS:
				dataRefLight.update({topLight: veri[2]});
				break;
			case RIGTH_DOOR_SW:
				dataRefLight.update({rightDoorSw: veri[2]});
				break;
			case LEFT_DOOR_SW:
				dataRefLight.update({leftDoorSw: veri[2]});
				break;
			default:
				//console.log("here");
				break;
	  }
  }
});

}, 1000);

//TCP SOCKET
	
var server=net.createServer(function(socket){ });

var aSocket, connected = false;
server.on("connection",function(socket){

    aSocket = socket;
    connected = true;

});

server.listen(1337, function()
{
	//console.log("TCP %j", server.address());
});

var database = admin.database();

var dataRefHor = database.ref('/horizon');
var dataRefBattery = database.ref('/battery');
var dataRefCabin = database.ref('/cabin');
var dataRefMotorRight = database.ref('/rightMotor');
var dataRefMotorLeft=database.ref('/leftMotor');
var dataRefLight=database.ref('/lights');

setTimeout(function(){
	setInterval(function (){

		dataRefCabin.set({
			inTemp  : all["valueCabin" ]["inTemp"],
			outTemp : all["valueCabin" ]["outTemp"],
			//SolKapı : all["valueCabin" ]["solKapı"],
			//SagKapı : all["valueCabin" ]["sagKapı"],
			//Flasher :all["valueCabin"]["flasher"],
			//Tavan   : all["valueCabin" ]["tavan"],
			h2Sens  : all["valueCabin"]["h2Sens"],
		});

		dataRefBattery.set({
			cell1 : all["valueBattery"]["cell0"],
			cell2 : all["valueBattery"]["cell1"],
			cell3 : all["valueBattery"]["cell2"],
			cell4 : all["valueBattery"]["cell3"],
			cell5 : all["valueBattery"]["cell4"],
			cell6 : all["valueBattery"]["cell5"],
			cell7 : all["valueBattery"]["cell6"],
			cell8 : all["valueBattery"]["cell7"],
			cell9 : all["valueBattery"]["cell8"],
			cell10 : all["valueBattery"]["cell9"],
			cell11 : all["valueBattery"]["cell10"],
			cell12 : all["valueBattery"]["cell11"],
			cell13 : all["valueBattery"]["cell12"],
			cell14 : all["valueBattery"]["cell13"],
			cell15 : all["valueBattery"]["cell14"],
			temp1 : all["valueBattery"]["temp1"],
			temp2 : all["valueBattery"]["temp2"],
			current : all["valueBattery"]["current"],
			batteryTemp : all["valueBattery" ]["batteryTemp"],

		});

		dataRefHor.set({
			status : all["valueHor"]["status"],
			ambientTemp : all["valueHor"]["ambientTemp"],
			stackVoltage : all["valueHor"]["stackVoltage"],
			H2Voltage : all["valueHor"]["H2Voltage"],
			stackTemp : all["valueHor"]["stackTemp"],
			current : all["valueHor"]["current"],
			batteryVoltage : all["valueHor"]["batteryVoltage"],
			
		});

		dataRefMotorRight.set({
			
			RightMotorSpeed :all["valueMotorRight"]["RightMotorSpeed"],
			MotorDriverRInput :all["valueMotorRight"]["MotorDriverRInput"],
			MotorDriverROutput :all["valueMotorRight"]["MotorDriverROutput"],
			MotorDriverRVINPUT :all["valueMotorRight"]["MotorDriverRVINPUT"] ,
			MotorDriverRVOUTPUT :all["valueMotorRight"]["MotorDriverRVOUTPUT"],
			MotorRight : all["valueMotorRight" ]["motorRightTemp"],
		
		});

		dataRefMotorLeft.set({
			LeftMotorSpeed :all["valueMotorLeft"]["LeftMotorSpeed"],
			MotorDriverLInput :all["valueMotorLeft"]["MotorDriverLInput"],
			MotorDriverLOutput :all["valueMotorLeft"]["MotorDriverLOutput"],
			MotorDriverLVINPUT :all["valueMotorLeft"]["MotorDriverLVINPUT"] ,
			MotorDriverLVINPUT :all["valueMotorLeft"]["MotorDriverLVOUTPUT"],
			MotorLeftTemp : all["valueMotorLeft" ]["motorLeftTemp"],
		});

/*
		dataRefLight.set({
			short :all["valueLight"]["short"],
			long :all["valueLight"]["long"],
			right :all["valueLight"]["right"] ,
			left :all["valueLight"]["left"],
			neoRight : all["valueLight" ]["neoRight"],
			neoLeft : all["valueLight" ]["neoLeft"],
			wiper : all["valueLight" ]["wiper"],
		});*/
		
	}, 1000);

},3000);

var SHORT_LIGHT = 0x09;
var SHORT_LIGHT_ANS = 0x10;

var LONG_LIGHT = 0x11;
var LONG_LIGHT_ANS = 0x12;

var LEFT_SIGNAL = 0x13;
var LEFT_SIGNAL_ANS = 0x14;

var RIGHT_SIGNAL = 0x15;
var RIGHT_SIGNAL_ANS = 0x16;

var SIGNAL = 0x17;
var SIGNAL_ANS = 0x18;

var NEO_PIXEL1 = 0x19;
var NEO_PIXEL1_ANS = 0x20;

var NEO_PIXEL2 = 0x21;
var NEO_PIXEL2_ANS = 0x22;

var WIPER = 0x23;
var WIPER_ANS = 0x24;

var TOP_LIGHT = 0x25;
var TOP_LIGHT_ANS = 0x26;

var RIGTH_DOOR_SW = 0x27;
var LEFT_DOOR_SW =0x28;