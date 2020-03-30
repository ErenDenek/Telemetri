
var cArray = [];
var all = {};

all["valueBattery"]["temp1"] = 10;
all["valueBattery"]["temp2"] = 40;
all["valueHor"]["stackTemp"] = 50;
all["valueHor"]["stackTemp"] = 5;

cArray.push("tempBattery" ,all["valueBattery"]["temp1"]);
cArray.push("tempBattery", all["valueBattery"]["temp2"]);
cArray.push("tempBattery", all["valueHor"]["stackTemp"]);
cArray.push("tempBattery", all["valueHor"]["ambientTemp"]);

cArray.sort(function(a, b){return a.tempBattery - b.tempBattery});

console.log(cArray);