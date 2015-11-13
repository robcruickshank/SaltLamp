// Neopixel Salt Lamp v1.0
// By Chris Barrett - Polaris Interplanetary - Special Projects Division
// Supports two modes; All pixel colour fade, and 8 pixel RGB mixer. All modes are asynchronous, and compartmentalized into functions.

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

int potVal = 9;                                     // integer to store the value from the potentiometer
int potPin = 2;                                     // integer to store the potentiometer pin
int butpin = 3;                                     // integer to store button pin
int butval;                                         // Button digital read variable
int butsel = 1;                                     // Button mode selection variable.
int butbounce;
int rcount;                                         // Counter for red
int gcount;                                         // Counter for green
int bcount;                                         // Counter for blue
int rval;                                           // int stores red brightness
int gval;                                           // int stores green brightness
int bval;                                           // int stores blue brightness
int rgo;                                            // red go int, 0 for off, 1 for upwards, 2 for downwards
int ggo;                                            // green go int, 0 for off, 1 for upwards, 2 for downwards
int bgo;                                            // blue go int, 0 for off, 1 for upwards, 2 for downwards
int magicnumber;                                    // magic number determines when to fire colours


//--------------------------------------------------// The following values are for the "Shimmer" routine
int pixelvalue[7];                                  // array to store the address location of each pixel event, 8 in total
int countval[7];                                    // array to store the counter for each pixel event, 8 in total
int pixelgo[7];                                     // array to store the activation bit for each pixel event, 8 in total
int pixmax = 512;                                   // int to store the max brightness for each pixel event
int pixelcol[7];                                    // array to store the colour decision
int pixcoldic[7];                                   // array to store the colour decision bit
int pixmat[7];                                      // array to indicate whether a pixel is stepping on another pixel
int delayval;                                       // integer to hold delay value
int randosparkle =0;                                // integer to hold the random number value
int randosparklenum[] = {1,2,3,4,5,6,7,8};          // integer to hold the magic numbers governing which pixel fires next
int maxbright;                                      // integer to determine max bright

void setup() {                                      //-----------------------------------------------(Setup)
    pinMode(butpin, INPUT);                         // Set buttonpin as input
    Serial.begin(9600);                             // Initialize Serial Debug
    pixels.begin();                                 // This initializes the NeoPixel library.
}

void loop() {                                       //-----------------------------------------------(Main Loop)
    pixels.setBrightness(255);
    
    butcheck();

    if (butsel == 1){
        clearscreen();
        colourfade();
    }

if (butsel == 2){
        clearscreen();
        shimmer();
    }
    
    Serial.println(butsel);
    pixels.show();                                  // This sends the updated pixel color to the hardware.
    delay(40);
  
}

void clearscreen(){                                 //-----------------------------------------------(clears the strip)
    for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    }  
}

void butcheck(){                                    //-----------------------------------------------(Button Check Function)
    pinMode(butpin, INPUT);
    butval = digitalRead(butpin);                   // check button pin for a high
    
    if (butval == HIGH){                            // if high signal read add one to the button selection variable
      butbounce = 1;
    }

    if (butbounce == 1 && butval == LOW){
     butsel = butsel + 1;
     butbounce = 0; 
    }

    if (butsel > 2){                               //  resets the button value variable. I use this method so I can add more modes if I want later.
    butsel = 1;
    }
}

void colourfade(){                                  //-----------------------------------------------(Colour Fade Function)
   
    
    magicnumber = random(0,30);                     //creates a magic number

    if (magicnumber == 3 && rgo == 0){              //checks to see if magic number has selected red and red is not currently firing
      rgo = 1;                                      //sets the red go bit to 1 meaning positive alternation
    }
    
    if (magicnumber == 10 && ggo == 0){              //checks to see if magic number has selected green
      ggo = 1;
    }
    
    if (magicnumber == 20 && bgo == 0){             //checks to see if magic number has selected blue
      bgo = 1;        
    }


    // ---------------------------------------------// Write Red
      
    if (rgo == 1){                                  // checks for ascent stage bit, adds one to red brightness 
      rval = rval + 1;
    }
    
    if (rgo == 2){                                  // checks for descent stage bit, subtracts one from brightness
      rval = rval - 1;
    }
    
    if (rval >= 255){                               // sets red bit to 2 meaning negative alternation
      rgo = 2;
    }
    
    if (rval == 0 && rgo == 2){                     // when counter runs out and go bit is 2, reset everything.
      rgo = 0;
      rval = 0;
    }

    // ---------------------------------------------// Write Blue
      
    if (bgo == 1){                                  // checks for ascent stage bit, adds one to blue brightness 
      bval = bval + 1;
    }
    
    if (bgo == 2){                                  // checks for descent stage bit, subtracts one from brightness
      bval = bval - 1;
    }
    
    if (bval >= 255){
      bgo = 2;
    }
    
    if (bval == 0 && bgo == 2){
      bgo = 0;
      bval = 0;
    }

    // ---------------------------------------------// Write Green
      
    if (ggo == 1){                                  // checks for ascent stage bit, adds one to green brightness 
      gval = gval + 1;
    }
    
    if (rgo == 2){                                  // checks for descent stage bit, subtracts one from brightness
      gval = gval - 1;
    }
    
    if (gval >= 255){
      ggo = 2;
    }
    
    if (gval == 0 && ggo == 2){
      ggo = 0;
      gval = 0;
    }
    
    
    potVal = analogRead(potPin);                    //determine delay
    potVal = map(potVal, 0, 1024, 0, 500);
    
    
    for(int i=LOWPIXELS;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(rval,gval,bval));
    }

  
}

void shimmer() {                                    //-----------------------------------------------(Shimmer Function)

    randosparkle = random(0,8);                      //creates a magic number
    
                                                    
    for (int i=0; i <= 7; i++){                      //For loop to check random number against magic number 
        if (randosparkle == randosparklenum[i] && pixelgo[i] == 0){ 
            pixelgo[i] = 1;    
        }
    } 
    
    for (int i=0; i <= 7; i++){                      //For loop to activate a pixel
        if (pixelgo[i] == 1 && pixcoldic[i] >= 0 && pixcoldic[i] < 1){                       
            drawPixelred(i);
        }
        if (pixelgo[i] == 1 && pixcoldic[i] >= 1 && pixcoldic[i] < 6){                       
            drawPixelgreen(i);
        }
          if (pixelgo[i] == 1 && pixcoldic[i] >= 6 && pixcoldic[i] < 9){                       
            drawPixelblue(i);
        }
}
    pixels.show();                                  // This sends the updated pixel color to the hardware.
}

void drawPixelred(int pixelv){                      //-----------------------------------------------(Red Pixel Draw Function)
 
 if(pixmat[pixelv]=0){
  for (int i=0; i <= 5; i++){
      if (pixelvalue[pixelv]!=pixelvalue[i] && pixelvalue[pixelv]<NUMPIXELS)
        pixelvalue[pixelv]=pixelvalue[pixelv]+1;
        pixmat[pixelv]=1;
      }
 
    }
 

  if (countval[pixelv] < pixmax){                                   //check if counter is within envelope
    if (countval[pixelv] < pixmax/2){                               //check if counter is within first half of fade
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(countval[pixelv],0,0));
    }

    if (countval[pixelv] > pixmax/2){                               //check if counter is within second half of fade    
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(-countval[pixelv],0,0));
    }
   countval[pixelv] = countval[pixelv] + 1;                         //increment counter
   
  }
  else {                                                            //when finished fading 
  pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,0));    //turn selected pixel off
  pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);             //create new random pixel location
  pixelgo[pixelv] = 0;                                              //deactivate pixel activate bit
  countval[pixelv] = 0;                                             //reset counter
  pixcoldic[pixelv] = random(0,9);
  pixmat[pixelv]=0;
}
  
 } 
 
void drawPixelgreen(int pixelv){                    //-----------------------------------------------(Green Pixel Draw Function 2)
  
   if(pixmat[pixelv]=0){                                //check for duplicates
  for (int i=0; i <= 5; i++){
      
    if (pixelvalue[pixelv]!=pixelvalue[i] && pixelvalue[pixelv]<NUMPIXELS)
        pixelvalue[pixelv]=pixelvalue[pixelv]+1;
        pixmat[pixelv]=1;
    }
 
    }
    
  

  if (countval[pixelv] < pixmax){                                   //check if counter is within envelope
    if (countval[pixelv] < pixmax/2){                               //check if counter is within first half of fade
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,countval[pixelv],0));
    }

    if (countval[pixelv] > pixmax/2){                               //check if counter is within second half of fade    
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,-countval[pixelv],0));
    }
   countval[pixelv] = countval[pixelv] + 1;                         //increment counter
   
  }
  else {                                                            //when finished fading 
  pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,0));    //turn selected pixel off
  pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);             //create new random pixel location
  pixelgo[pixelv] = 0;                                              //deactivate pixel activate bit
  countval[pixelv] = 0;                                             //reset counter
  pixcoldic[pixelv] = random(0,9);
  pixmat[pixelv]=0;
}
  
 } 
 
void drawPixelblue(int pixelv){                     //-----------------------------------------------(Blue Pixel Draw Function 3)
 
 if(pixmat[pixelv]=0){
  for (int i=0; i <= 5; i++){
      if (pixelvalue[pixelv]!=pixelvalue[i] && pixelvalue[pixelv]<NUMPIXELS)
        pixelvalue[pixelv]=pixelvalue[pixelv]+1;
        pixmat[pixelv]=1;
      }
 
    }
 

  if (countval[pixelv] < pixmax){                                   //check if counter is within envelope
    if (countval[pixelv] < pixmax/2){                               //check if counter is within first half of fade
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,countval[pixelv]));
    }

    if (countval[pixelv] > pixmax/2){                               //check if counter is within second half of fade    
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,-countval[pixelv]));
    }
   countval[pixelv] = countval[pixelv] + 1;                         //increment counter
   
  }
  else {                                                            //when finished fading 
  pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,0));    //turn selected pixel off
  pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);             //create new random pixel location
  pixelgo[pixelv] = 0;                                              //deactivate pixel activate bit
  countval[pixelv] = 0;                                             //reset counter
  pixcoldic[pixelv] = random(0,9);
  pixmat[pixelv]=0;
}
  
 } 
 
