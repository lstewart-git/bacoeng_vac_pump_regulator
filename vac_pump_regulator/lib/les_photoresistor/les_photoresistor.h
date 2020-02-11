/*
les photoresistor driver
this  photoresistor maintains internal
state which can be queried

 */

#ifndef les_photoresistor_h
#define les_photoresistor_h
#include "Arduino.h"

class les_photoresistor{

	public:
		// seems constructors with no arguments dont work with arduino compiler
		les_photoresistor (int pin_num);
		void Setup ();
		void Update ();
		int photoresistor_value;
		int my_pin;

};


#endif
