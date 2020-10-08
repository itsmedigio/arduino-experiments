#include <Adafruit_NeoPixel.h> // Include for NeoPixel LED Ring
#include <Wire.h>
#include <Adafruit_GFX.h> // Include for display
#include <Adafruit_SSD1306.h>
#include <DHT.h>
   
#define LED_PIN 3 // Where LED ring is connected

#define LED_COUNT 20 // How many LEDs in the ring

#define DHTPIN  2 // Where humi&temp sensor is connected

#define DHTTYPE DHT11  // CHANGE THIS IF YOU ARE USING A DIFFERENT TYPE OF DHT SENSOR

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // DEFINE YOUR STRIP
 
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire); // Type of display used


/*##################################### MQTT'S CONFIG ######################################*/

#define BROKER_IP    "YOURIP"
#define DEV_NAME     "mqttdevice"
#define MQTT_USER    "test"
#define MQTT_PW      "test"
const char ssid[] = "YOURSSID"; //Change this to your SSID
const char pass[] = "YOURPASS"; //Change this to your Wi-Fi password

/*############################### FINE CONFIGUURAZIONE MQTT #################################*/

/*############################### WHERE WE DEFINE OUR BOARD ##########################*/

#include <MQTT.h>
#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#else
#error unknown board
#endif

/*############################### END OF DEFINE  ################################*/

DHT dht(DHTPIN, DHTTYPE);                // Configure DHT library
 
char temperature[] = "00.0 C";
char humidity[]    = "00.0 %";

const int buttonPin = 0; // the number of the pushbutton pin
int buttonState = 0; // button state

WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

int mode = 0; // Mode of the lamp
int brightness = 50; // Brightness
String payload2; // I use this to copy the payload for later use



/* ##################### Wi-Fi Functions ############################*/

void connect() {
  Serial.print("Checking Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nConnecting to MQTT broker");
  while (!client.connect(DEV_NAME, MQTT_USER, MQTT_PW)) {
    Serial.print(".");
    delay(1000);
  }
  client.subscribe("/controllo"); //SUBSCRIBE TO TOPIC /controllo
  client.subscribe("/readtemp"); //SUBSCRIBE TO TOPIC /readtemp
  client.subscribe("/readhumi"); //SUBSCRIBE TO TOPIC /readhumi
  Serial.println("\nConnection is good! Yay!");
}

/* ##################### End Wi-Fi Functions ############################*/

void messageReceived(String &topic, String &payload) {
  Serial.println("\nRicevo: " + payload);
  if (payload == "off") {
    Serial.println("DEBUG: off");
    mode = 0;
  }
  else if (payload == "red") {
    Serial.println("DEBUG: red");
    mode = 1;
  }
  else if (payload == "green") {
    Serial.println("DEBUG: green");
    mode = 2;
  }
  else if (payload == "blue") {
    Serial.println("DEBUG: blue");
    mode = 3;
  }
  else if (payload == "white") {
    Serial.println("DEBUG: white");
    mode = 4;
  }
  else if (payload == "incrbr") {
    Serial.println("DEBUG: INCREASE BRIGHTNESS");
    if (brightness+25 <= 255)
    brightness += 25;
    strip.setBrightness(brightness);
  }
  else if (payload == "redbr") {
    Serial.println("DEBUG: REDUCE BRIGHTNESS");
    if (brightness-18 >= 0)
      brightness -= 18;
    strip.setBrightness(brightness);
  }
  else {
    Serial.println(payload);
    mode = 5;
    payload2 = payload;
  }
}


/* ################### La lampada ############## */

void setup() {
  pinMode(buttonPin, INPUT);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(brightness); // Set BRIGHTNESS
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin(BROKER_IP, 1883, net);
  client.onMessage(messageReceived);
  mode = 4; //Inizialize this at WHITE
  connect();
  Serial.println("\nBooting completed!");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  dht.begin();                // Initialize the DHT library
  display.clearDisplay();
  display.drawFastHLine(0, 32, SSD1306_LCDWIDTH, WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  client.loop();
  client.onMessage(messageReceived);
  switch (mode) {          
    case 0:
      colorWipe(strip.Color(  0,   0,   0));    // Black/off
      break;
    case 1:
      colorWipe(strip.Color(255,   0,   0));    // Red
      break;
    case 2:
      colorWipe(strip.Color(  0, 255,   0));    // Green
      break;
    case 3:
      colorWipe(strip.Color(  0,   0, 255));    // Blue
      break;
    case 4:
      colorWipe(strip.Color(  255,  255, 255));    // White
      break;
    case 5:
      String hexstring = payload2;
      Serial.println("DEBUG HEX:" + hexstring);
      unsigned long int number = (long) strtol( &hexstring[0], NULL, 16);
      int red = number >> 16;
      int green = number >> 8 & 0xFF;
      int blue = number & 0xFF;
      Serial.println(red);
      colorWipe(strip.Color(  red,  green, blue));  //User defined colour
      break;
  }
  byte RH = dht.readHumidity();
  //Read temperature in degree Celsius
  byte Temp = dht.readTemperature();
  display.setCursor(0,0); 
  display.clearDisplay();
  temperature[0] = Temp / 10 + 48;
  temperature[1] = Temp % 10 + 48;
  humidity[0]    = RH / 10 + 48;
  humidity[1]    = RH % 10 + 48;
  display.print("T: ");
  display.print(temperature);
  display.print("\nH: ");
  display.drawFastHLine(0, 32, SSD1306_LCDWIDTH, WHITE);
  display.print(humidity);
  display.display();
  client.publish("/datitemp", temperature);
  client.publish("/datihum", humidity);
}


void colorWipe(uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}
