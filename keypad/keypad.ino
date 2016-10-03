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


const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
//byte rowPins[ROWS] = { 9, 8, 7, 6 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
//byte colPins[COLS] = { 12, 11, 10 }; 

// REMAPPED CONNECTION
byte rowPins[ROWS] = { 12, 7, 9, 10 };
byte colPins[COLS] = { 11, 8, 6 }; 


// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

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
   
  char key = kpd.getKey();
  int reading = digitalRead(key);

  
  if(key)  // Check for a valid key.
  {
    Serial.println(reading);
    switch (key)
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
        break;
      case '2':
        tone(melodyPin, NOTE_B3,noteDuration);
        break;
      case '3':
        tone(melodyPin, NOTE_C4,noteDuration);
        break;
      case '4':
        tone(melodyPin, NOTE_D4,noteDuration);
        break;
      case '5':
        tone(melodyPin, NOTE_E4,noteDuration);
        break;
      case '6':
        tone(melodyPin, NOTE_F4,noteDuration);
        break;
      case '7':
        tone(melodyPin, NOTE_G4,noteDuration);
        break;
      case '8':
        tone(melodyPin, NOTE_A4,noteDuration);
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


        break;
      case '*':
        count++;
        if (count%2==1)
            analogWrite(motorPin, 200); 
        else
            analogWrite(motorPin, 0);
        break;
      case '#':
        analogWrite(motorPin, 0);
        break;
      default:
        Serial.println(key);
    }
  }
}

