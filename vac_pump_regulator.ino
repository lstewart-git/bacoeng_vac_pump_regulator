// Optically driven vacuum pump regulator
// requires another device in the vacuum chamber:
// optical_vac_pump_sensor_bombe
//
// by Lawrence E Stewart
// Copyleft 2018, GNU GPL v3.0

// Arduino Connection Layout:
// digital pin 0 : 
// digital pin 1 : 
// digital pin 2 : Button 1
// digital pin 3 : Button 2
// digital pin 4 : 
// digital pin 5 : 
// digital pin 6 : Tx Software Serial GPS
// digital pin 7 : Rx Software Serial GPS
// digital pin 8 : 
// digital pin 9 : 
// digital pin 10 : 
// digital pin 11 : 
// digital pin 12 : pump power relay
// digital pin 13 : 
// analog pin 0 : 
// analog pin 1 : cds photocell
// analog pin 2 :
// analog pin 3 :
// analog pin 4 : I2C SDA oled display
// analog pin 5 : I2C SCL oled display

// custom libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <les_button_v2.h>
#include <les_photoresistor.h>

//instantiate breadboard objects
les_button_v2 button1(2, 350, 3);  //mode changer
les_button_v2 button2(5, 350, 11);  //trigger level changer
les_photoresistor myCDS(A1);  // chamber sensor
//instantiate an oled_display object
#define OLED_RESET 12
Adafruit_SSD1306 les_screen(OLED_RESET);


// program logic variables
static const int relay_pin = 12; // pump power relay
int light_val = 0; // current cds light reading
int init_light_val = 0; // light seen in setup
int maxval = 0; // max light val
int minval = 1024;  // min light val
int trigger_level = 1090; // light on/off discriminator
int total_pump_cycles = 0;
bool pump_started = false;
long int pump_start_time = 0;
long int pump_stop_time = 0;
long int total_pumpin_time_ms = 0;

//   ////////////    SETUP     /////////////////////////////
void setup()
{
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  les_screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  button1.Setup();
  button2.Setup();
  myCDS.Setup();
  // get initial light level
  myCDS.Update();
  init_light_val = myCDS.photoresistor_value;
  // set trigger above init value
  trigger_level = init_light_val + 50;
  show_logo();
}
//   ///////////////// MAIN PROGRAM LOOP     //////////////
void loop()
{
      // update and read all components
      myCDS.Update();
      light_val = myCDS.photoresistor_value;
      button1.Update();
      button2.Update();
      
      // change the trigger level as necc
      if (button1.state_flag == 0){
      // keep pump power off in state 0 (initial setup)  
      digitalWrite(relay_pin, LOW);
      trigger_level = init_light_val + 50 + (button2.state_flag * 50); 
        if ( trigger_level > 1023 ){
          trigger_level = init_light_val + 50;
        }
      }
      
      // keep track of light val range
      if (light_val > maxval) maxval = light_val;
      if (light_val < minval) minval = light_val;

      // check for operational mode:
      if ( button1.state_flag != 0){
   
          // light is off inside, so no pumping
          if (light_val < trigger_level){
            digitalWrite(relay_pin, LOW); // turn off pump
            // detect pump shut down this cycle
            if (pump_started == true){
              pump_started = false;
              pump_stop_time = millis();
              // integrate total runtime:
              total_pumpin_time_ms += (pump_stop_time - pump_start_time);
            }
          }
      
          // light is on inside, so run pump
          else {
            digitalWrite(relay_pin, HIGH); // turn on pump
            // detect pump turn on this cycle
            if (pump_started == false){
              pump_started = true;
              pump_start_time = millis();
              total_pump_cycles += 1;
            }
           
          }

       } // end operational mode check

     
      // pick a screen to display
      if ( button1.state_flag == 0) show_logo();
      if ( button1.state_flag == 1) display_mode1();
      if ( button1.state_flag == 2) display_mode2();
	
        
 } // ////////////////  END MAIN PROGRAM LOOP   /////////////////

void display_mode1()
{
    les_screen.clearDisplay();
    les_screen.setCursor(0,0);
    les_screen.setTextSize(1);
    les_screen.print("max:");
    les_screen.print(maxval);
    les_screen.print(" min:");
    les_screen.println(minval);
    les_screen.print("trigger level:");
    les_screen.println(trigger_level);
    les_screen.setTextSize(2);
    les_screen.println(light_val);
    les_screen.display();
} 

void display_mode2()
{
    les_screen.clearDisplay();
    les_screen.setCursor(0,0);
    les_screen.setTextSize(1);
    les_screen.print("RunTime(sec):");
    les_screen.println(millis()/1000);
    les_screen.print("PumpTime(sec):");
    les_screen.println(total_pumpin_time_ms/1000);
    les_screen.print("Duty Cycle:");
    les_screen.println(float(total_pumpin_time_ms)/float(millis()));
    les_screen.print("Total Cycles:");
    les_screen.println(total_pump_cycles);
    les_screen.display();
} 


void show_logo(){
  les_screen.clearDisplay();
  les_screen.setTextSize(1);
  les_screen.setTextColor(WHITE);
  les_screen.setCursor(0,0);
  les_screen.println("Pump Regulator");
  les_screen.print("Current Light:");
  les_screen.println(light_val);
  les_screen.print("Trigger:");
  les_screen.println(trigger_level);
  les_screen.println("Start=Right Button");
  les_screen.display();
}


// END PROGRAM
