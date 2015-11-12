// Salt Lamp neopixel lights random colour crossfade
// By Chris Barrett - Polaris Interplanetary - Special Projects Division

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      133
#define LOWPIXELS      87

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
int potVal = 9;                                     //integer to store the value from the potentiometer
int potPin = 2;                                     //integer to store the potentiometer pin
int rcount;                                         // Counter for red
int gcount;                                         // Counter for green
int bcount;                                         // Counter for blue
int rval;                                           //int stores red brightness
int gval;                                           //int stores green brightness
int bval;                                           //int stores blue brightness
int rgo;                                            // red go int, 0 for off, 1 for upwards, 2 for downwards
int ggo;                                            // green go int, 0 for off, 1 for upwards, 2 for downwards
int bgo;                                            // blue go int, 0 for off, 1 for upwards, 2 for downwards
int magicnumber;                                    // magic number determines when to fire colours

void setup() {

Serial.begin(9600); 
  pixels.begin();                                   // This initializes the NeoPixel library.
}

void loop() {                                       

magicnumber = random(0,3);                          //creates a magic number

if (magicnumber == 1 && rgo == 0){                  //checks to see if magic number has selected red and red is not currently firing
  rgo = 1;                                          //sets the red go bit to 1 meaning positive alternation
}

if (magicnumber == 2 && ggo == 0){                   //checks to see if magic number has selected green
  ggo = 1;
}

if (magicnumber == 1 && bgo == 0){                   //checks to see if magic number has selected blue
  bgo = 1;        
}
  
if (rgo == 1){                                      //checks for ascent stage bit, adds one to red brightness 
  rval = rval + 1;
}

if (rgo == 2){
  rval = rval - 1;
}

if (rval >= 255){
  rgo == 2;
}

if (rval == 0 && rgo == 2){
  rgo = 0;
  rval = 0;
}

potVal = analogRead(potPin);                     //determine delay
potVal = map(potVal, 0, 1024, 0, 500);


for(int i=LOWPIXELS;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(rval,bval,gval));
}

  
pixels.show(); // This sends the updated pixel color to the hardware.

Serial.println(rval);

  
delay(potVal);
}
