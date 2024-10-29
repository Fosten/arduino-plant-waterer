/****************************************************
 Arduino Plant Waterer
 Arduino 4 Terminal Mechanical Relay
 By Brian Davis
 Created 2020.04.11

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

//Declare moisture measurements
const int AirValue = 568;
const int WaterValue = 300;
int intervals = (AirValue - WaterValue)/3;
//Pins are controled by A0, A1, A2, A3.
const int sensor1 = A0;
const int sensor2 = A1;
const int sensor3 = A2;
const int sensor4 = A3;
//Set the initial sensor values.
int soilMoistureValue1 = 0;
int soilMoistureValue2 = 0;
int soilMoistureValue3 = 0;
int soilMoistureValue4 = 0;
//Pins are controled by 10, 11, 12, 13.
const int relay1 = 10;
const int relay2 = 11;
const int relay3 = 12;
const int relay4 = 13;

void setup() {
  //Set relay pins as output
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  //Set the initial relay values.
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  // open serial port, set the baud rate to 9600 bps
  Serial.begin(9600);
}

// Run a loop on Sensor 1 & Trigger Relay 1 if needed.
void loop() {
    soilMoistureValue1 = analogRead(sensor1);  //put Sensor insert into soil
    Serial.println((String)"Sensor 1 =" + soilMoistureValue1);
    // Wet From About 260 - 346.66
    if(soilMoistureValue1 > WaterValue && soilMoistureValue1 < (WaterValue + intervals))
    {
      Serial.println((String)"Sensor 1: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay1, HIGH);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue1 < AirValue && soilMoistureValue1 > (AirValue - intervals))
    {
      Serial.println((String)"Sensor 1: Very Dry.  Let's pump some water.  Glug, glug glug...");
      digitalWrite(relay1, LOW);
    }
    delay(1500);

    soilMoistureValue2 = analogRead(sensor2);  //put Sensor insert into soil
    Serial.println((String)"Sensor 2 =" + soilMoistureValue2);
    // Wet From About 260 - 346.66
    if(soilMoistureValue2 > WaterValue && soilMoistureValue2 < (WaterValue + intervals))
    {
      Serial.println((String)"Sensor 2: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay2, HIGH);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue2 < AirValue && soilMoistureValue2 > (AirValue - intervals))
    {
      Serial.println((String)"Sensor 2: Very Dry.  Let's pump some water.  Glug, glug glug...");
      digitalWrite(relay2, LOW);
    }
    delay(1500);

    soilMoistureValue3 = analogRead(sensor3);  //put Sensor insert into soil
    Serial.println((String)"Sensor 3 =" + soilMoistureValue3);
    // Wet From About 260 - 346.66
    if(soilMoistureValue3 > WaterValue && soilMoistureValue3 < (WaterValue + intervals))
    {
      Serial.println((String)"Sensor 3: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay3, HIGH);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue3 < AirValue && soilMoistureValue3 > (AirValue - intervals))
    {
      Serial.println((String)"Sensor 3: Very Dry.  Let's pump some water  Glug, glug glug...");
      digitalWrite(relay3, LOW);
    }
    delay(1500);

    soilMoistureValue4 = analogRead(sensor4);  //put Sensor insert into soil
    Serial.println((String)"Sensor 4 =" + soilMoistureValue4);
    // Wet From About 260 - 346.66
    if(soilMoistureValue4 > WaterValue && soilMoistureValue4 < (WaterValue + intervals))
    {
      Serial.println((String)"Sensor 4: Very Wet.  Time to stop pumping water.  Spigot shut off.");
      digitalWrite(relay4, HIGH);
    }
    // Dry from about 434 - 520
    else if(soilMoistureValue4 < AirValue && soilMoistureValue4 > (AirValue - intervals))
    {
      Serial.println((String)"Sensor 4: Very Dry.  Let's pump some water  Glug, glug glug...");
      digitalWrite(relay4, LOW);
    }
    delay(1500);
}
