
#include "Arduino.h"

#include "les_button_v2.h"


les_button_v2::les_button_v2(int pin_num, int debounce, int modes){
  butt_pin = pin_num;
	state_flag = 0;
	debounce_millis = debounce;
  num_modes = modes;
}

  void les_button_v2::Setup()
  {
	 pinMode(butt_pin, INPUT_PULLUP);
  }


  void les_button_v2::Update()
  {
    // check to see if it's time to change internal state
  unsigned long currentMillis = millis();
	int buttval = digitalRead(butt_pin);
	// check that debounce timer fm last press expired
	if (currentMillis > last_press_time + debounce_millis){

		if (buttval == LOW) {     // button is pressed
			last_press_time = currentMillis;
            state_flag ++;  // increment the 'state' or 'mode'
      // recycle state to 1, state 0 is the startup state
			if (state_flag >= num_modes) state_flag = 1;
			}
		}
   }
