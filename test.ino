#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
Adafruit_VEML6070 uv = Adafruit_VEML6070();

int adc_key_val[5] ={50, 200, 400, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1; 
int oldkey=-1;

// Define variables for start time and operating duration
long start = 0;
long t = 0;
long dt = 0;
int minutes = 0;
int seconds = 0;

void setup()
{

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("T 00m00s I 0.000");
  lcd.setCursor(0, 1);
  lcd.print("Dose 0.0000");

  // Initiliaze the UV sensor and set integration time to 500ms
  uv.begin(VEML6070_4_T);

}

//setup button fuctions
//void check_buttons()


// Convert ADC value to key number
int get_key(unsigned int input)
{
    int k;
   
    for (k = 0; k < NUM_KEYS; k++)
    {
      if (input < adc_key_val[k])
 {
            return k;
        }
   }
   
    if (k >= NUM_KEYS)k = -1;  // No valid key pressed
    return k;
}

void loop()
{
  adc_key_in = analogRead(0);
  key = get_key(adc_key_in); 
  lcd.setCursor(0,1); 
   switch (key)
   { 
    case -1:
     lcd.print("      ");
     break;
    case 0:
     lcd.print("Right");
     break;
   }
}
