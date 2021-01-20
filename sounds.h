#pragma once
#include <Arduino.h>

class Sounds {
  public:
    /*
     * Create obj, if pin is not specified - then 
     * create it on D4 pin.
    */
    Sounds(byte buzzerPin = 4);
    /*
     * Play melody by it`s ID:
     * 0 - Hello melody;
     * -----------------------
     * 1 - Low battery
     * 2 - Medium-low battery
     * 3 - Medium-high battery
     * 4 - Height battery
     * -----------------------
     * 5 - Lost connection
    */
    void play(byte melody);
  private:
    byte _buzzerPin;
};
