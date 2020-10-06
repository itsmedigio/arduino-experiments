#include <Adafruit_NeoPixel.h>
#define LED_PIN 3
#define LED_COUNT 20
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

/*##################################### CONF DI MQTT ######################################*/

#define BROKER_IP    "192.168.1.57"
#define DEV_NAME     "mqttdevice"
#define MQTT_USER    "test"
#define MQTT_PW      "test"
const char ssid[] = "TIM-19640501";
const char pass[] = "gstSYZuwdfIaAwnVq5knVcaj";

/*############################### FINE CONFIGUURAZIONE MQTT ####################*/

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

WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

const int buttonPin = 2; // the number of the pushbutton pin
int buttonState = 0; //stato bottone

int mode = 0; // Questa variabile controlla il colore
int brightness = 50; // Inizializzo luminosita' a circa 1/3
String payload2; // Mi serve per l'rgb

/* ##################### Funzioni connessione  ############################*/

void connect() {
  Serial.print("Controllo Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nConnessione alla rete...");
  while (!client.connect(DEV_NAME, MQTT_USER, MQTT_PW)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nCollegato correttamente!");
  client.subscribe("/controllo"); //SUBSCRIBE TO TOPIC /controllo
  Serial.println("\nTopic collegato!");
}

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
  Serial.println("\nAvvio/Reset lampada");
  WiFi.begin(ssid, pass);
  client.begin(BROKER_IP, 1883, net);
  client.onMessage(messageReceived);
  mode = 4; //Inizialize this at WHITE
  connect();
}

void loop() {
  client.loop();
  if (buttonState == HIGH) {
  client.onMessage(messageReceived);
  }
  switch (mode) {          // Start the new animation...
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
      unsigned long int number = (long) strtol( &hexstring[1], NULL, 16);
      int red = number >> 16;
      int green = number >> 8 & 0xFF;
      int blue = number & 0xFF;
      Serial.println(red);
      colorWipe(strip.Color(  red,  green, blue));  //User defined colour
      break;
  }
}


void colorWipe(uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}
