
// Neopixel Salt Lamp v1.1
// By Chris Barrett - Polaris Interplanetary - Special Projects Division
// Supports two modes; All pixel colour fade, and 8 pixel RGB mixer. All modes are asynchronous, and compartmentalized into functions.
// Changes ----------
// v1.1 added a new system to test for and reject pixels that are using an already selected location on the strip during the shimmer function to hopefully reduce flickering.


//#include <Adafruit_NeoPixel.h>
//#include <Encoder.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      133
#define LOWPIXELS      87



// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Encoder myEnc(2, 3);                                // Encoder pins, these are hardware interrupt pins so you have to use the right one for your particular Arduino
int butpin = 5;                                     // integer to store button pin
int butval;                                         // Button digital read variable
int butsel = 1;                                     // Button mode selection variable.
int butbounce;                                      // button debounce
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
byte pixelvalue[7];                                  // array to store the address location of each pixel event, 8 in total
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
long oldPosition  = -999;


void setup() {                                      //-----------------------------------------------(Setup)
    pinMode(butpin, INPUT);                         // Set buttonpin as input
    Serial.begin(9600);                             // Initialize Serial Debug
    //pixels.begin();                                 // This initializes the NeoPixel library.
}

void loop() {                                       //-----------------------------------------------(Main Loop)
  
  butcheck();

  if (butsel == 0){
      clearscreen();
      colourfade();
  }
  
  if (butsel == 1){
      clearscreen();
      shimmer();
  }
  
  Serial.println(butsel);
  //pixels.show();                                  // This sends the updated pixel color to the hardware.
     
     
 for (int i=0; i <= 7; i++){                      //For loop to check random number against magic number 
        
             Serial.print(" pixel ");
             Serial.print(i);
             Serial.print(" is");
             Serial.println(pixelvalue[i]);    
       
 } 
  


}

void clearscreen(){                                 //-----------------------------------------------(clears the strip)
    for(int i=0;i<NUMPIXELS;i++){
        //pixels.setPixelColor(i, pixels.Color(0,0,0));
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

    if (butsel > 1){                               //  resets the button value variable. I use this method so I can add more modes if I want later.
    butsel = 0;
    }
}

void colourfade(){                                  //-----------------------------------------------(Colour Fade Function)

    //long newPosition = myEnc.read();
    //if (newPosition != oldPosition) {
    //    oldPosition = newPosition;
    //    Serial.println(newPosition);
   // }
    //if (newPosition > 255){
    //  myEnc.write(1);
    //}
   // if (newPosition < 1){
   //   myEnc.write(255);
   // }
    
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
    
    

    
    for(int i=LOWPIXELS;i<NUMPIXELS;i++){
        //pixels.setPixelColor(i, pixels.Color(rval,gval,bval));
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
           // drawPixelgreen(i);
        }
          if (pixelgo[i] == 1 && pixcoldic[i] >= 6 && pixcoldic[i] < 9){                       
           // drawPixelblue(i);
        }
}
    //pixels.show();                                  // This sends the updated pixel color to the hardware.
}

void drawPixelred(int pixelv){                      //-----------------------------------------------(Red Pixel Draw Function)

  if (countval[pixelv]==0){                                                           // If statement to test if this ID is stepping on anyone, first it checks if the fade has just started (to reduce flickering)

    pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);                                 // Create new random pixel location
    
      if (pixelv==0){                                                                 // If the current pixel ID is 0 test every following ID location against randomly generated pixel location.
        for (int i=1; i == 7; i++){          
          if (pixelvalue[i]==pixelvalue[pixelv]){
          pixmat[pixelv]=pixmat[pixelv]+1;
          }
        }
      }
  
      if (pixelv==7){                                                                 // If the current pixel ID is 7 test every previous ID location.
        for (int i=0; i < 7; i++){
            if (pixelvalue[i]==pixelvalue[pixelv]){
              pixmat[pixelv]=pixmat[pixelv]+1;
            }
          }
        }
  
  
      if (pixelv > 0 && pixelv < 7){                                                  // For IDs somewhere in the middle, test every ID that isn't the current ID
          
        for (int i=0; i < pixelv; i++){ 
          if (pixelvalue[i]==pixelvalue[pixelv]){
            pixmat[pixelv]=pixmat[pixelv]+1;
            }
        }
  
        for (int i=pixelv+1; i == 7; i++){
          if (pixelvalue[i]==pixelvalue[pixelv]){
            pixmat[pixelv]=pixmat[pixelv]+1;
            }
        }
      
      }

    Serial.println(pixmat[pixelv]);
   }

  if (pixmat[pixelv] == 0){                                                           //check if match variable is zero, meaning no other pixel is using currently selected position. If it is the position will be discarded.

    Serial.print(countval[pixelv]);
    if (countval[pixelv] < pixmax){                                                   //check if counter is within envelope
    
      if (countval[pixelv] < pixmax/2){                                               //check if counter is within first half of fade
        //pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(countval[pixelv],0,0));
      }
      
      if (countval[pixelv] > pixmax/2){                                               // check if counter is within second half of fade    
        //pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(-countval[pixelv],0,0));
      }
      
      countval[pixelv] = countval[pixelv] + 1;                                        // Increment counter each pass through

    }
    else {                                                                            // when finished fading 
      pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);                               //create new random pixel location
      pixelgo[pixelv] = 0;                                                            //deactivate pixel activate bit
      countval[pixelv] = 0;                                                           //reset counter
      pixcoldic[pixelv] = random(0,9);
      pixmat[pixelv]=0;
    }
    
   }
  else{
    
    pixmat[pixelv]==0;                                                                // If the pixel is trying to use another pixels position reset pixmat in order to allow the whole thing to start over again next time through and hopefully find a number not in use?
    
  }

} 
 
void drawPixelgreen(int pixelv){                    //-----------------------------------------------(Green Pixel Draw Function 2)
  if (countval[pixelv]==0){                                                           // If statement to test if this ID is stepping on anyone, first it checks if the fade has just started (to reduce flickering)

    pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);                                 // Create new random pixel location
    
      if (pixelv==0){                                                                 // If the current pixel ID is 0 test every following ID location against randomly generated pixel location.
        for (int i=1; i = 7; i++){          
          if (pixelvalue[i]==pixelvalue[pixelv]){
          pixmat[pixelv]=pixmat[pixelv]+1;
          }
        }
      }
  
      if (pixelv==7){                                                                 // If the current pixel ID is 7 test every previous ID location.
        for (int i=0; i < 7; i++){
            if (pixelvalue[i]==pixelvalue[pixelv]){
              pixmat[pixelv]=pixmat[pixelv]+1;
            }
          }
        }
  
  
      if (pixelv > 0 && pixelv < 7){                                                  // For IDs somewhere in the middle, test every ID that isn't the current ID
          
        for (int i=0; i < pixelv; i++){ 
          if (pixelvalue[i]==pixelvalue[pixelv]){
            pixmat[pixelv]=pixmat[pixelv]+1;
            }
        }
  
        for (int i=pixelv+1; i = 7; i++){
          if (pixelvalue[i]==pixelvalue[pixelv]){
            pixmat[pixelv]=pixmat[pixelv]+1;
            }
        }
      
      }
  
   }

  if (pixmat[pixelv] == 0){                                                           //check if match variable is zero, meaning no other pixel is using currently selected position. If it is the position will be discarded.
    
    if (countval[pixelv] < pixmax){                                                   //check if counter is within envelope
    
      if (countval[pixelv] < pixmax/2){                                               //check if counter is within first half of fade
        //pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,countval[pixelv],0));
      }
      
      if (countval[pixelv] > pixmax/2){                                               // check if counter is within second half of fade    
        //pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,-countval[pixelv],0));
      }
      
      countval[pixelv] = countval[pixelv] + 1;                                        // Increment counter each pass through
      
    }
    else {                                                                            // when finished fading 
      pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);                               //create new random pixel location
      pixelgo[pixelv] = 0;                                                            //deactivate pixel activate bit
      countval[pixelv] = 0;                                                           //reset counter
      pixcoldic[pixelv] = random(0,9);
      pixmat[pixelv]=0;
    }
    
   }
  else{
    
    pixmat[pixelv]==0;                                                                // If the pixel is trying to use another pixels position reset pixmat in order to allow the whole thing to start over again next time through and hopefully find a number not in use?
    
  }
 } 
 
void drawPixelblue(int pixelv){                     //-----------------------------------------------(Blue Pixel Draw Function 3)
 if (countval[pixelv]==0){                                                           // If statement to test if this ID is stepping on anyone, first it checks if the fade has just started (to reduce flickering)

    pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);                                 // Create new random pixel location
    
      if (pixelv==0){                                                                 // If the current pixel ID is 0 test every following ID location against randomly generated pixel location.
        for (int i=1; i = 7; i++){          
          if (pixelvalue[i]==pixelvalue[pixelv]){
          pixmat[pixelv]=pixmat[pixelv]+1;
          }
        }
      }
  
      if (pixelv==7){                                                                 // If the current pixel ID is 7 test every previous ID location.
        for (int i=0; i < 7; i++){
            if (pixelvalue[i]==pixelvalue[pixelv]){
              pixmat[pixelv]=pixmat[pixelv]+1;
            }
          }
        }
  
  
      if (pixelv > 0 && pixelv < 7){                                                  // For IDs somewhere in the middle, test every ID that isn't the current ID
          
        for (int i=0; i < pixelv; i++){ 
          if (pixelvalue[i]==pixelvalue[pixelv]){
            pixmat[pixelv]=pixmat[pixelv]+1;
            }
        }
  
        for (int i=pixelv+1; i = 7; i++){
          if (pixelvalue[i]==pixelvalue[pixelv]){
            pixmat[pixelv]=pixmat[pixelv]+1;
            }
        }
      
      }
  
   }

  if (pixmat[pixelv] == 0){                                                           //check if match variable is zero, meaning no other pixel is using currently selected position. If it is the position will be discarded.
    
    if (countval[pixelv] < pixmax){                                                   //check if counter is within envelope
    
      if (countval[pixelv] < pixmax/2){                                               //check if counter is within first half of fade
        //pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,countval[pixelv]));
      }
      
      if (countval[pixelv] > pixmax/2){                                               // check if counter is within second half of fade    
        //pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,-countval[pixelv]));
      }
      
      countval[pixelv] = countval[pixelv] + 1;                                        // Increment counter each pass through
      
    }
    else {                                                                            // when finished fading 
      pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);                               //create new random pixel location
      pixelgo[pixelv] = 0;                                                            //deactivate pixel activate bit
      countval[pixelv] = 0;                                                           //reset counter
      pixcoldic[pixelv] = random(0,9);
      pixmat[pixelv]=0;
    }
    
   }
  else{
    
    pixmat[pixelv]==0;                                                                // If the pixel is trying to use another pixels position reset pixmat in order to allow the whole thing to start over again next time through and hopefully find a number not in use?
    
  }
  
 } 
 
