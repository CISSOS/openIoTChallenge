 //Create a client instance ("location.hostname", Number(location.port), "clientID")
client = new Paho.MQTT.Client("0.0.0.0", Number(1883), "ClientID");

// ! The URL of the Thing
var thingUrl = "https://Url-to-Ditto.com/api/2/things/come.your.namespace:YOURTHING/";

// ! The Basic Authorization string (Encode your Username and Passwort with Base64)
var authorization = "Basic xxxxxxxxxxxxxxxxxx";

// ! The cr-api-token
var apiToken = "xxxxxxxxxxxxxxxxxxxxx";



var count = 0; // variable that not every message gets send to the twin

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});

// called when client connects
function onConnect() {
  console.log("onConnect");
  // make a subscription to the path where your data gets send, # means you will listen to everything
  client.subscribe("#");
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  count = count + 1;
  // set data as message
  var data = message.payloadString;
  // To use the data you have to convert the JSON-Object into a JavaScript-Object with JSON.parse()
  var obj = JSON.parse(data);
  // Now we can get every sepcific property, but to send it to IoT-Things we have to change it back to JSON with JSON.stringify()
  var acc = JSON.stringify(obj.acc);
  var mag = JSON.stringify(obj.mag);
  var gyro = JSON.stringify(obj.gyro);
  var temp = JSON.stringify(obj.temperature);
  var hum = JSON.stringify(obj.humidity);
  var pres = JSON.stringify(obj.pressure);
  var lig = JSON.stringify(obj.light);
  // we only want every third JSON to be send to IoT-Things
if(count % 3 == 0){
  // here the HTTP-Request gets created with the necessary headers
    var xhttp1 = new XMLHttpRequest();
    xhttp1.open("PUT", thingUrl+"acc", true);
    xhttp1.setRequestHeader('Authorization', authorization);
    xhttp1.setRequestHeader("Content-Type", "application/json");
    xhttp1.setRequestHeader("x-cr-api-token", apiToken);
    xhttp1.send(acc); // here the data gets send
    var xhttp2 = new XMLHttpRequest();
    xhttp2.open("PUT", thingUrl+"mag", true);
    xhttp2.setRequestHeader('Authorization', authorization);
    xhttp2.setRequestHeader("Content-Type", "application/json");
    xhttp2.setRequestHeader("x-cr-api-token", apiToken);
    xhttp2.send(mag);
    var xhttp3 = new XMLHttpRequest();
    xhttp3.open("PUT", thingUrl+"gyro", true);
    xhttp3.setRequestHeader('Authorization', authorization);
    xhttp3.setRequestHeader("Content-Type", "application/json");
    xhttp3.setRequestHeader("x-cr-api-token", apiToken);
    xhttp3.send(gyro);
    var xhttp4 = new XMLHttpRequest();
    xhttp4.open("PUT", thingUrl+"temperature", true);
    xhttp4.setRequestHeader('Authorization', authorization);
    xhttp4.setRequestHeader("Content-Type", "application/json");
    xhttp4.setRequestHeader("x-cr-api-token", apiToken);
    xhttp4.send(temp);
    var xhttp5 = new XMLHttpRequest();
    xhttp5.open("PUT", thingUrl+"humidity", true);
    xhttp5.setRequestHeader('Authorization', authorization);
    xhttp5.setRequestHeader("Content-Type", "application/json");
    xhttp5.setRequestHeader("x-cr-api-token", apiToken);
    xhttp5.send(hum);
    var xhttp6 = new XMLHttpRequest();
    xhttp6.open("PUT", thingUrl+"pressure", true);
    xhttp6.setRequestHeader('Authorization', authorization);
    xhttp6.setRequestHeader("Content-Type", "application/json");
    xhttp6.setRequestHeader("x-cr-api-token", apiToken);
    xhttp6.send(pres);
    var xhttp7 = new XMLHttpRequest();
    xhttp7.open("PUT", thingUrl+"light", true);
    xhttp7.setRequestHeader('Authorization', authorization);
    xhttp7.setRequestHeader("Content-Type", "application/json");
    xhttp7.setRequestHeader("x-cr-api-token", apiToken);
    xhttp7.send(lig);
  }
}
