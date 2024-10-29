/****************************************************
 Arduino Plant Waterer
 WiFi Arduino 4 Terminal SSR MQTT
 By Brian Davis
 Created 2023.02.05

 This program reads Capacitive Soil Moisture Sensors,
 measures soilMoistureValue vs AirValue vs WaterValue,
 and then triggers a relay module to switch
 water pumps on/off when needed.

 GNU General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

// Wifi include
#include <SPI.h>
#include <WiFiNINA_Generic.h>

// MQTT include
#include <ArduinoMqttClient.h>

// Wifi Setup
char ssid[] = "YOUR_SSID_HERE";
char pass[] = "YOUR_PASSWORD_HERE";
int keyIndex = 0;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

//MQTT Setup
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "YOUR_MQTT_BROKER_LOCAL_IP_ADDRESS_HERE"; // replace with your credentials
int        port     = 1883;
const char topic1[]  = "soil_moisture_sensor_1";
const char topic2[]  = "soil_moisture_sensor_2";
const char topic3[]  = "soil_moisture_sensor_3";
const char topic4[]  = "soil_moisture_sensor_4";
const char topic5[]  = "water_pump_relay_state_1";
const char topic6[]  = "water_pump_relay_state_2";
const char topic7[]  = "water_pump_relay_state_3";
const char topic8[]  = "water_pump_relay_state_4";

// replace with your credentials
#define BROKER_USERNAME     "YOUR_MQTT_BROKER_USERNAME_HERE"
#define BROKER_PASSWORD     "YOUR_MQTT_BROKER_PASSWORD_HERE"

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

/*  The default AirValue and WaterValue for some sensors are 520 and 260.
 *  To calibrate your sensor, run this code and open the Serial Monitor.
 *  Record the value being measured as your new AirValue.
 *  Insert the sensor to the white line in a cup of water.
 *  Record the new reading as the WaterValue.
 */

//Declare moisture measurements
const int AirValue = 568;
const int WaterValue = 300;
int intervals = (AirValue - WaterValue)/3;
//Pins are controlled by A0, A1, A2, A3.
const int sensor1 = A0;
const int sensor2 = A1;
const int sensor3 = A2;
const int sensor4 = A3;
//Set the initial sensor values.
int soilMoistureValue1 = 0;
int soilMoistureValue2 = 0;
int soilMoistureValue3 = 0;
int soilMoistureValue4 = 0;
//Pins are controlled by 10, 11, 12, 13.
const int relay1 = 10;
const int relay2 = 11;
const int relay3 = 12;
const int relay4 = 13;
//Set the initial sensor values.
volatile byte waterpumpRelayState1 = LOW;
volatile byte waterpumpRelayState2 = LOW;
volatile byte waterpumpRelayState3 = LOW;
volatile byte waterpumpRelayState4 = LOW;

void setup() {
  //Set relay pins as output
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  //Set the initial relay values.
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  // open serial port, set the baud rate to 9600 bps
  Serial.begin(9600);
  // Wifi setup
  while (status != WL_CONNECTED) {
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, pass);
  delay(10000);
  }
  server.begin();

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println("You're connected to the network");
  Serial.println();

//You can provide a username and password for authentication
  mqttClient.setUsernamePassword(BROKER_USERNAME, BROKER_PASSWORD);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

//Run a loop on the MQTT}
//Run a loop on Sensor 1 & Trigger Relay 1 if needed.

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //record random value from A0, A1 and A2
    soilMoistureValue1 = analogRead(sensor1);
    soilMoistureValue2 = analogRead(sensor2);
    soilMoistureValue3 = analogRead(sensor3);
    soilMoistureValue4 = analogRead(sensor4);
    waterpumpRelayState1 = digitalRead(relay1);
    waterpumpRelayState2 = digitalRead(relay2);
    waterpumpRelayState3 = digitalRead(relay3);
    waterpumpRelayState4 = digitalRead(relay4);

    Serial.print("Sending message to topic: ");
    Serial.println(topic1);
    Serial.println(soilMoistureValue1);

    Serial.print("Sending message to topic: ");
    Serial.println(topic2);
    Serial.println(soilMoistureValue2);

    Serial.print("Sending message to topic: ");
    Serial.println(topic3);
    Serial.println(soilMoistureValue3);

    Serial.print("Sending message to topic: ");
    Serial.println(topic4);
    Serial.println(soilMoistureValue4);

    Serial.print("Sending message to topic: ");
    Serial.println(topic5);
    Serial.println(waterpumpRelayState1);

    Serial.print("Sending message to topic: ");
    Serial.println(topic6);
    Serial.println(waterpumpRelayState2);

    Serial.print("Sending message to topic: ");
    Serial.println(topic7);
    Serial.println(waterpumpRelayState3);

    Serial.print("Sending message to topic: ");
    Serial.println(topic8);
    Serial.println(waterpumpRelayState4);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic1);
    mqttClient.print(soilMoistureValue1);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(soilMoistureValue2);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print(soilMoistureValue3);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic4);
    mqttClient.print(soilMoistureValue4);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic5);
    mqttClient.print(waterpumpRelayState1);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic6);
    mqttClient.print(waterpumpRelayState2);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic7);
    mqttClient.print(waterpumpRelayState3);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic8);
    mqttClient.print(waterpumpRelayState4);
    mqttClient.endMessage();

    Serial.println();
  }

  //Wifi loop
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {

          if (currentLine.length() == 0) {

            client.println("Arduino Plant Waterer 1");

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    soilMoistureValue1 = analogRead(sensor1);  //put Sensor insert into soil
    client.println((String)"Sensor 1 =" + soilMoistureValue1);
    // Wet From About 260 - 346.66
    if(soilMoistureValue1 > WaterValue && soilMoistureValue1 < (WaterValue + intervals))
    {
      client.println((String)"Sensor 1: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay1, LOW);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue1 < AirValue && soilMoistureValue1 > (AirValue - intervals))
    {
      client.println((String)"Sensor 1: Very Dry.  Let's pump some water.  Glug, glug glug...");
      digitalWrite(relay1, HIGH);
    }
    delay(1500);

    soilMoistureValue2 = analogRead(sensor2);  //put Sensor insert into soil
    client.println((String)"Sensor 2 =" + soilMoistureValue2);
    // Wet From About 260 - 346.66
    if(soilMoistureValue2 > WaterValue && soilMoistureValue2 < (WaterValue + intervals))
    {
      client.println((String)"Sensor 2: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay2, LOW);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue2 < AirValue && soilMoistureValue2 > (AirValue - intervals))
    {
      client.println((String)"Sensor 2: Very Dry.  Let's pump some water.  Glug, glug glug...");
      digitalWrite(relay2, HIGH);
    }
    delay(1500);

    soilMoistureValue3 = analogRead(sensor3);  //put Sensor insert into soil
    client.println((String)"Sensor 3 =" + soilMoistureValue3);
    // Wet From About 260 - 346.66
    if(soilMoistureValue3 > WaterValue && soilMoistureValue3 < (WaterValue + intervals))
    {
      client.println((String)"Sensor 3: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay3, LOW);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue3 < AirValue && soilMoistureValue3 > (AirValue - intervals))
    {
      client.println((String)"Sensor 3: Very Dry.  Let's pump some water  Glug, glug glug...");
      digitalWrite(relay3, HIGH);
    }
    delay(1500);

    soilMoistureValue4 = analogRead(sensor4);  //put Sensor insert into soil
    client.println((String)"Sensor 4 =" + soilMoistureValue4);
    // Wet From About 260 - 346.66
    if(soilMoistureValue4 > WaterValue && soilMoistureValue4 < (WaterValue + intervals))
    {
      client.println((String)"Sensor 4: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay4, LOW);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue4 < AirValue && soilMoistureValue4 > (AirValue - intervals))
    {
      client.println((String)"Sensor 4: Very Dry.  Let's pump some water  Glug, glug glug...");
      digitalWrite(relay4, HIGH);
    }
    delay(1500);
    client.stop();
    Serial.println("client disconnected");
  }
}
