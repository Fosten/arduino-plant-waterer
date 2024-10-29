/****************************************************
 Arduino Plant Waterer
 Feather 1 Terminal Mechanical Relay
 By Brian Davis
 Created 2020.03.16

 This program reads Capacitive Soil Moisture Sensors,
 measures soilMoistureValue vs AirValue vs WaterValue,
 and then triggers a relay module to switch
 water pumps on/off when needed.

 GNU General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/*  The default AirValue and WaterValue for some sensors are 520 and 260.
 *  To calibrate your sensor, run this code and open the Serial Monitor.
 *  Record the value being measured as your new AirValue.
 *  Insert the sensor to the white line in a cup of water.
 *  Record the new reading as the WaterValue.
 */

//Moisture Sensor
const int AirValue = 567;
const int WaterValue = 300;
int intervals = (AirValue - WaterValue)/3;
int soilMoistureValue = 0;

// Sensor pin is controled by A0
int sensor_pin = A0;

// Relay pin is controled by 14
int relay = 14;
volatile byte relayState = LOW;

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  pinMode(relay, OUTPUT);
}

void LedState(int state) {
  digitalWrite(relay, HIGH);
}

void loop() {
  soilMoistureValue = analogRead(sensor_pin)*3.5;  //put Sensor insert into soil
  Serial.println(soilMoistureValue);

// From 260 - 346.66
if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
{
Serial.println("Very Wet");
  LedState(HIGH);
}

//About 434 - 520
else if(soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
{
  Serial.println("Dry");
  LedState(LOW);
}
delay(500);
}
