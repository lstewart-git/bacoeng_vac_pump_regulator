#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "LowPower.h"

/*    You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
   
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

*/
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// program variables
float min_press = 1200.0;
int pump_on = 0;   // pump state 0 = off


void setup(void) 
{
  digitalWrite(LED_BUILTIN, LOW);

  // LED PINS
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  /* Initialise the sensor */
  if(!bmp.begin())
  {
   digitalWrite(4, HIGH); // turn on 1 led
    while(1);  // stays here forever!
  }
  
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
  if (event.pressure){
    
    if (!pump_on && event.pressure > 800) {   // Pump is off, press high
      pump_on = 1; // we're turning pump on
      digitalWrite(3, HIGH);  // led on
      digitalWrite(4, HIGH);  // led on
      }


    if (pump_on && event.pressure < 600) {   // Pump is on, press low
      pump_on = 0; // we're turning pump off
      digitalWrite(3, LOW);  // led off
      digitalWrite(4, LOW);  // led off
      }

    if (!pump_on && event.pressure < 800) {   // Pump is off, press low
      // Go to low power sleep for 2 sec
      LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
      }
      
    } // END event.pressure

  } // END LOOP
  
