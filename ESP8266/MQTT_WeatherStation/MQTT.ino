/*
   Measure temperature, humidity and pressure with a BME280 I2C
*/

#include <Wire.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Ticker.h>

#define WLAN_SSID       "Put_your_own_here"
#define WLAN_PASS       "Put_your_own_here"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Put_your_own_here"
#define AIO_KEY         "Put_your_own_here"

#define SEALEVELPRESSURE_HPA (1013.25)

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup the feeds temperature, humidity, and pressure
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish pressureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pressure");

Adafruit_BME280 bme; // I2C

Ticker readSensorData;
Ticker sendSensorData;

//Variables to hold current average
float vTemp = 0.0F;
float vHum = 0.0F;
float vPress = 0.0F;

//Variables to hold the vaild count
int vTempValid = 0;
int vHumValid = 0;
int vPressValid = 0;

//Number of entries for calculating the average and the entries itself
const int arraysize = 3;

float vTempA[arraysize];
float vHumA[arraysize];
float vPressA[arraysize];

//foward declaration
void MQTT_connect();


// the setup
void setup() {
  Serial.begin(9600);
  delay(10);
  Wire.begin();

  if (! bme.begin(0x76, &Wire)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  readSensorData.attach(10.0F, readData);
  sendSensorData.attach(30.0F, sendMQTTData);
}

//the loop
void loop() {
  MQTT_connect();
  delay(30000);
}


/*
   Connect to the MQTT broker
*/
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 10;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}


/*
   Read the sensor data and save it
*/
void readData() {
  Serial.println(F("Reading sensor data "));

  for (int i = arraysize - 1; i > 0; i--) {
    vTempA[i] = vTempA[i - 1];
    vHumA[i] = vHumA[i - 1];
    vPressA[i] = vPressA[i - 1];
  }

  vTempA[0] = bme.readTemperature();
  if (isnan(vTempA[0])) {
    vTempValid = 0;
  } else
  {
    vTempValid++;
    vTempValid = min(vTempValid, arraysize);
  }

  vHumA[0] = bme.readHumidity();
  if (isnan(vHumA[0])) {
    vHumValid = 0;
  } else
  {
    vHumValid++;
    vHumValid = min(vHumValid, arraysize);
  }

  vPressA[0] = bme.readPressure() / 100.0F;
  if (isnan(vPressA[0])) {
    vPressValid = 0;
  } else
  {
    vPressValid++;
    vPressValid = min(vPressValid, arraysize);
  }

  //calculate sensor average if possible
  if (vTempValid == arraysize) {
    vTemp = 0.0F;
    for (int i = 0; i < arraysize; i++) {
      vTemp += vTempA[i];
    }
    vTemp /= 3;
    Serial.println("T= " + String(vTemp));
  } else {
    Serial.println("T= not valid");
  }

  if (vHumValid == arraysize) {
    vHum = 0.0F;
    for (int i = 0; i < arraysize; i++) {
      vHum += vHumA[i];
    }
    vHum /= 3;
    Serial.println("H= " + String(vHum));
  } else {
    Serial.println("H= not valid");
  }

  if (vPressValid == arraysize) {
    vPress = 0.0F;
    for (int i = 0; i < arraysize; i++) {
      vPress += vPressA[i];
    }
    vPress /= 3;
    Serial.println("P= " + String(vPress));
  } else {
    Serial.println("P= not valid");
  }
}

/*
 * Send the sensor data to the feeds
 */
void sendMQTTData() {
  MQTT_connect();

  if (vTempValid == arraysize) {
    Serial.print(F("\nSending temperatureFeed val "));
    Serial.print(vTemp);
    Serial.print("...");
    if (! temperatureFeed.publish(vTemp)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  } else {
    Serial.println(F("Not sending temperature data due to error"));
  }

  if (vHumValid == arraysize) {
    Serial.print(F("\nSending humidityFeed val "));
    Serial.print(vHum);
    Serial.print("...");
    if (! humidityFeed.publish(vHum)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  } else {
    Serial.println(F("Not sending humidity data due to error"));
  }

  if (vPressValid == arraysize) {
    Serial.print(F("\nSending pressureFeed val "));
    Serial.print(vPress);
    Serial.print("...");
    if (! pressureFeed.publish(vPress)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  } else {
    Serial.println(F("Not sending pressure data due to error"));
  }
}
