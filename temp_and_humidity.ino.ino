// Arduino with SSD1306 OLED and DHT11 relative humidity and temperature sensor.
 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>                         // Include DHT library code
 
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
 
#define DHTPIN  2                        // DHT11 data pin is connected to Arduino pin 8
#define DHTTYPE DHT11                  // DHT11 sensor is used

DHT dht(DHTPIN, DHTTYPE);                // Configure DHT library
 
char temperature[] = "00.0 C";
char humidity[]    = "00.0 %";
 
void setup(void) {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  display.display();
  delay(1000);
  dht.begin();                // Initialize the DHT library
  // Clear the display buffer.
  display.clearDisplay();
  display.drawFastHLine(0, 32, SSD1306_LCDWIDTH, WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
}
 
void loop() {
  // Read humidity
  byte RH = dht.readHumidity();
  //Read temperature in degree Celsius
  byte Temp = dht.readTemperature();
  display.setCursor(0,0); 
  display.clearDisplay();
  temperature[0] = Temp / 10 + 48;
  temperature[1] = Temp % 10 + 48;
  humidity[0]    = RH / 10 + 48;
  humidity[1]    = RH % 10 + 48;
  //display.print("Temperatura:");
  display.print("T: ");
  display.print(temperature);
  display.print("\nH: ");
  display.drawFastHLine(0, 32, SSD1306_LCDWIDTH, WHITE);
  //display.print("\nUmidita':");
  display.print(humidity);
  display.display();
  delay(1000);
 
}
