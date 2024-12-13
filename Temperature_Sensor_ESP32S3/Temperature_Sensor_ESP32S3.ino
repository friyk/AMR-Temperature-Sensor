/* Temperature Sensor v1.0

This device runs off an ESP32S3 (Model: VND-GND YD-ESP32-S3 Type-A-V1.5). The temperature sensor detects the contact temperature, 
which the ESP determines if it is higher than the set threshold value. If so, a warning sound will be played by the attached speaker.

*/

// Libraries used:
/// Temperature Sensor
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
/// Speaker
#include <DFRobotDFPlayerMini.h>
/// Screen
#include <Adafruit_SSD1306.h>

// SSD1306 128x64 Screen
#define ScreenWidth 128
#define ScreenHeight 64
#define OLED_RESET 1    // Arbitrary unused pin

Adafruit_SSD1306 display(ScreenWidth, ScreenHeight, &Wire, OLED_RESET);

// DS18B20 Temperature Sensor
#define TempSensor 4
float warningTemp = 30.00;      //Adjust this value to change warning threshold (Up to 2 Decimal Places)

OneWire oneWire(TempSensor);
DallasTemperature sensors(&oneWire);

// DFR0299 DFPlayerMini
DFRobotDFPlayerMini myDFPlayer;


void setup() {
  Serial.begin(115200);
  

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {    // Must have! 0x3C should be changed depending on I2C connection (Use I2C Scanner to Check)
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial1.begin(9600);        //Use 2nd Hardware Serial Port
  myDFPlayer.begin(Serial1);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
  myDFPlayer.volume(30);

  // Starting Screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,20);
  display.println(F("Temperature"));
  display.setCursor(85,45);
  display.println(F("Sensor"));
  display.display();

  delay(3000);




}

void loop() {


  display.clearDisplay();
  sensors.requestTemperatures();    // Read temperature from sensor

  Serial.print("Celsius:");
  float tempCel = sensors.getTempCByIndex(0);
  Serial.println(tempCel);



  if (tempCel > warningTemp){       // Check if below threshold temperature
    myDFPlayer.play(2);

  }

  // Update screen
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println(F("Temperature Sensor°C"));
  
  display.setCursor(20,20);
  display.setTextSize(3);
  display.println(tempCel);

  display.setCursor(10,50);
  display.setTextSize(1);
  display.println("Threshold =");
  display.setCursor(85,50);
  display.println(warningTemp);

  display.display();

  delay(300);

}
