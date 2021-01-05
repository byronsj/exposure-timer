#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
Adafruit_VEML6070 uv = Adafruit_VEML6070();

int adc_key_val[5] ={50, 200, 400, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1; 

// Define variables for start time and operating duration
long start = 0;
long time = 0;
long dose_time = 0;
int minutes = 0;
int seconds = 0;
int state = 1;
double alarm = 0;

// Define variables for current irradiance and dose
// The irradiance is normalized to be a percentage of
// the maximum sensor output, or 2^16-1
double I = 0;
double dose = 0;
const double I_norm = (double) 100/65535;
static char Istr[5];
void setup()
{

    // Initialize LCD
    lcd.begin(16, 2);

    // Initiliaze the UV sensor and set integration time to 500ms
    uv.begin(VEML6070_4_T);

}
//buzzer    
//TODO: set buuzzer pins and time

void timer() {
    if (alarm < 0) {
        state = 2;
    } else if (state == 0) {
        dose += I / 2;
        if (alarm > 0) {
            alarm -= I / 2;
        }
    }
}

//setup button fuctions
void check_buttons()
{
    switch(key)
    {
        case 0:
            //right
            if (state == 1) {
                alarm = 0;
                dose = 0;
                lcd.clear();
                //lcd.setCursor(11, 1);
                //lcd.print(dose);
            }
            delay(50);
            break;
        case 1:
            //up
            if (state == 1){
                alarm += 100;
            }
            delay(50);
            break;
        case 2:
            //down

            if (state == 1){
                alarm -= 100;
            }
            delay(50);
            break;
        case 3:
            //left
            if (state == 1){
                alarm += 500;
            }
            delay(50);
            break;
        case 4:
            //select
            if (state == 2)
            {
                state = 0;
                alarm = 0;
            } else if (state == 1) {
                state = 0;
            } else {
                state++;
            }
            delay(50);
            break;
    }
}
// update lcd
void update_lcd()
{
    lcd.setCursor(0,1);
    lcd.print(alarm);
    lcd.setCursor(11,1);
    lcd.print(dose);

    if ( state == 0){
        lcd.setCursor(0,0);
        lcd.print("run  ");
    } else if (state == 1){
        lcd.setCursor(0,0);
        lcd.print("set    ");
    } else if (state == 2){
        lcd.setCursor(0,0);
        lcd.print("buzzer ");
    }

    // Update LCD with irradiance
    I = (double) uv.readUV() * I_norm;
    lcd.setCursor(11,0);
    dtostrf(I, 5, 3, Istr);
    lcd.print(Istr);
}

//Convert ADC value to key number
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
    timer();
    check_buttons();
    update_lcd();
}
