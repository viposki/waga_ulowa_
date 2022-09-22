#include "core.hpp"

#include <DHT.h>
#include <HX711.h>
#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

float calibration_factor = -13900; //weight calibration factor

constexpr int INSIDE_DHT_SENSOR_PIN = 12;
constexpr uint8_t INSIDE_DHT_SENSOR_TYPE = DHT22; //DHT_SENSOR_TYPE = DHT22;

constexpr int OUTSIDE_DHT_SENSOR_PIN = 13;
constexpr uint8_t OUTSIDE_DHT_SENSOR_TYPE = DHT22; //DHT_SENSOR_TYPE = DHT22;

LiquidCrystal_I2C lcd(0x27, 16, 2); //create an lcd instance
HX711 scale; // create a weight instance

DHT insideSensor(INSIDE_DHT_SENSOR_PIN, INSIDE_DHT_SENSOR_TYPE); //DHT_SENSOR_TYPE = DHT22;
DHT outsideSensor(OUTSIDE_DHT_SENSOR_PIN, OUTSIDE_DHT_SENSOR_TYPE); //DHT_SENSOR_TYPE = DHT22;

void initializeDevices()
{
  lcd.begin();
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  insideSensor.begin();
  outsideSensor.begin();
}

void scaleReset()
{
  scale.set_scale();

  // Reset the scale to 0.
  scale.tare();
}

void showStartLCDText(String text)
{
  lcd.setCursor(3,0); // center the cursor to the text
  lcd.print(text);
}

void setup() 
{
  Serial.begin(9600);
  
  initializeDevices();

  scaleReset();

  showStartLCDText(F("Waga ulowa"));
  delay(1000);
  
  lcd.clear();
}
 
void loop() 
{
  delay(1000);
  lcd.clear();

  // Adjust to this calibration factor.
  scale.set_scale(calibration_factor);
 
  const float weight = scale.get_units(5);

  const float insideHumidity = insideSensor.readHumidity();
  const float insideTemperature = insideSensor.readTemperature();

  const float outsideHumidity = outsideSensor.readHumidity();
  const float outsideTemperature = outsideSensor.readTemperature();
 
  // LCD prints
  {
    lcd.setCursor(0, 0);
    lcd.print(core::fullPrettyValueText("Waga", weight, "kg"));

    delay(1000);
    lcd.clear();

    lcd.setCursor(0, 0); //first line
    lcd.print(core::fullPrettyValueText("H_in", insideHumidity, "%"));
    lcd.setCursor(0, 1); //second line
    lcd.print(core::fullPrettyValueText("T_in", insideTemperature, " C"));

    delay(1000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(core::fullPrettyValueText("H_out", outsideHumidity, "%"));
    lcd.setCursor(0, 1);
    lcd.print(core::fullPrettyValueText("T_out", outsideTemperature, " C"));
  }
  
  // Serial prints
  {
    Serial.println("---------------------------");
    Serial.println(core::fullPrettyValueText("Waga", weight, "kg"));
    Serial.println();
    Serial.println(core::fullPrettyValueText("Wewnetrzna wilgotnosc", insideHumidity, "%"));
    Serial.println(core::fullPrettyValueText("Wewnetrzna temperatura", insideTemperature, " C"));
    Serial.println();
    Serial.println(core::fullPrettyValueText("Zewnetrzna wilgotnosc", outsideHumidity, "%"));
    Serial.println(core::fullPrettyValueText("Zewnetrzna temperatura", outsideTemperature, " C"));
  }
}