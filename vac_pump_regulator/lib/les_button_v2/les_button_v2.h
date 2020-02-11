/*
les button driver
this button maintains internal
state or 'mode', initializing
at mode '0', then cycling through
modes 1 - num_modes repeatedly

 */

#ifndef les_button_h
#define les_button_h
#include "Arduino.h"

class les_button_v2{

	public:
		les_button_v2 (int pin_num, int debounce_millis, int modes);
		void Setup ();
		void Update ();
		int state_flag;


	private:
		unsigned long debounce_millis;
		unsigned long last_press_time = 0;
        int num_modes;
		int butt_pin;
};


#endif
