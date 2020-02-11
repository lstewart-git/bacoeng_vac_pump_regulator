
#include "Arduino.h"

#include "les_photoresistor.h"

les_photoresistor::les_photoresistor(int pin_num)
{
  my_pin = pin_num;
}

  void les_photoresistor::Setup()
  {
   photoresistor_value = analogRead(my_pin);
  }


  void les_photoresistor::Update()
  {
    photoresistor_value = analogRead(my_pin);
   }
