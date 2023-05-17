#include "HX711.h"

HX711 scale6;
//HX711 scale5;
HX711 scale4;
HX711 scale3;
HX711 scale2;
HX711 scale1;
float calibration_factor = -52000;

void setup() {
  Serial.begin(9600);
  scale6.begin(13,12); //dt 13 sck12 
  //scale5.begin(11,10); //dt sck 
  scale4.begin(9,8); //dt sck 
  scale3.begin(7,6); //dt sck 
  scale2.begin(5,4); //dt sck 
  scale1.begin(3,2); //dt sck 

  scale6.set_scale();
  //scale5.set_scale();
  scale4.set_scale();
  scale3.set_scale();
  scale2.set_scale();
  scale1.set_scale();
  
  scale6.tare(); //Reset the scale to 0
  //scale5.tare();
  scale4.tare();
  scale3.tare();
  scale2.tare();
  scale1.tare();
}

void loop() {
  
  scale1.set_scale(calibration_factor);
  scale2.set_scale(calibration_factor);
  scale3.set_scale(calibration_factor);

  scale4.set_scale(calibration_factor);
  //scale5.set_scale(calibration_factor);
  scale6.set_scale(calibration_factor);

  int num3 = int(scale3.get_units());
  int num4 = int(scale4.get_units());

  if(num3 > num4) {
    int k = 1;
    Serial.write(k);
  }
  
  delay(333);
}
