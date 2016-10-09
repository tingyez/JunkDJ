/*  FROM http://playground.arduino.cc/Main/KeypadTutorial
 *  
 *  MORE EXAMPLES http://playground.arduino.cc/Code/Keypad#Example
 *   
 *   Keypadtest.pde
 *
 *  Demonstrate the simplest use of the  keypad library.
 *
 *  The first step is to connect your keypad to the
 *  Arduino  using the pin numbers listed below in
 *  rowPins[] and colPins[]. If you want to use different
 *  pins then  you  can  change  the  numbers below to
 *  match your setup.
 *
 */
#include <Keypad.h>
#include "pitches.h"


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define ringPin 5
// How many NeoPixels are attached to the Arduino? (0-11)
#define ringPixels 12

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(ringPixels, ringPin, NEO_GRB + NEO_KHZ800);


const byte aROWS = 4; // Four rows
const byte aCOLS = 3; // Three columns
// Define the Keymap
char akeys[aROWS][aCOLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};

const byte bROWS = 1; // + one rows
const byte bCOLS = 3; // Three columns
// Define the Keymap
char bkeys[bROWS][bCOLS] = {
  {'a','b','c'}
};


// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
//byte rowPins[ROWS] = { 9, 8, 7, 6 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
//byte colPins[COLS] = { 12, 11, 10 }; 

// REMAPPED CONNECTION
byte arowPins[aROWS] = { 12, 7, 9, 10 };
byte acolPins[aCOLS] = { 11, 8, 6 }; 

byte browPins[aROWS] = { A0 };     //this one is not connected yet
byte bcolPins[aCOLS] = { 7, 9, 10 }; 


// Create the Keypad
Keypad kpdA = Keypad( makeKeymap(akeys), arowPins, acolPins, aROWS, aCOLS );
Keypad kpdB = Keypad( makeKeymap(bkeys), browPins, bcolPins, bROWS, bCOLS );


#define ledpin 13
int motorPin = 2;
int melodyPin = 4;

//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
//Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

  int count=0;

void setup()
{
  pinMode(ledpin,OUTPUT);
  pinMode(motorPin, OUTPUT);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600);

  while (! Serial);
  Serial.println("Speed 0 to 255");


  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
}

void loop()
{
//  if (Serial.available())
//  {
//    int speed = Serial.parseInt();
//    if (speed >= 0 && speed <= 255)
//    {
//      analogWrite(motorPin, speed);
//    }
//  }
  

  int noteDuration = 1000/8;
   
  char keyA = kpdA.getKey();
  char keyB = kpdB.getKey();
  
//  int reading = digitalRead(key);

  
  if(keyA)  // Check for a valid key.
  {
//    Serial.println(reading);
    switch (keyA)
    {
      // to calculate the note duration, take one second 
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      case '1':
        tone(melodyPin, NOTE_A3,noteDuration);
        delay(125);
        tone(melodyPin, NOTE_B3,noteDuration);
        delay(125);
        tone(melodyPin, NOTE_C3,noteDuration);
        Serial.println("1");

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(0, pixels.Color(0,150,0)); // 0 = LED position
        pixels.show(); // This sends the updated pixel color to the hardware.
    
        break;
      case '2':
        tone(melodyPin, NOTE_B3,noteDuration);
        Serial.println("2");
        break;
      case '3':
        tone(melodyPin, NOTE_C4,noteDuration);
        Serial.println("3");
        break;
      case '4':
        tone(melodyPin, NOTE_D4,noteDuration);
        Serial.println("4");
        break;
      case '5':
        tone(melodyPin, NOTE_E4,noteDuration);
        Serial.println("5");
        break;
      case '6':
        tone(melodyPin, NOTE_F4,noteDuration);
        Serial.println("6");
        break;
      case '7':
        tone(melodyPin, NOTE_G4,noteDuration);
        Serial.println("7");
        break;
      case '8':
        tone(melodyPin, NOTE_A4,noteDuration);
        Serial.println("8");
        break;
      case '9':
//        tone(melodyPin, NOTE_B4,noteDuration);
//        int size = sizeof(underworld_melody) / sizeof(int);
        for (int thisNote = 0; thisNote < 8; thisNote++) {
          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / underworld_tempo[thisNote];
     
          tone(melodyPin, underworld_melody[thisNote], noteDuration);
     
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
     
          // stop the tone playing:
          tone(melodyPin, 0, noteDuration);
        }
        Serial.println("9");
        break;
        
      case '0':
//        tone(melodyPin, NOTE_C5,noteDuration);
        Serial.println(" 'Mario Theme'");
//        int size = sizeof(melody) / sizeof(int);
        for (int thisNote = 0; thisNote < 12; thisNote++) {
          int noteDuration = 1000 / tempo[thisNote];
          tone(melodyPin, melody[thisNote], noteDuration);
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
     
          // stop the tone playing:
          tone(4, 0, noteDuration);
        }
        Serial.println("0");
        break;
        
      case '*':
        count++;
        if (count%2==1)
            analogWrite(motorPin, 200); 
        else
            analogWrite(motorPin, 0);
        Serial.println("*");
        break;
      case '#':
        analogWrite(motorPin, 0);
        Serial.println("#");
        break;
      default:
        Serial.println(keyA);

    }
  }

  if (keyB){

    switch(keyB){

      case 'a':
        Serial.println("a");
        break;
      case 'b':
        Serial.println("b");
        break;
      case 'c':
        Serial.println("c");
        break;
      default:
        Serial.println(keyB);
    }
  }
}

