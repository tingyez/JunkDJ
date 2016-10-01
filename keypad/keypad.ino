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

void setup()
{
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600);
}

void loop()
{

  int noteDuration = 1000/8;
   
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      // to calculate the note duration, take one second 
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      case '1':
        tone(4, NOTE_A3,noteDuration);
        break;
      case '2':
        tone(4, NOTE_B3,noteDuration);
        break;
      case '3':
        tone(4, NOTE_C4,noteDuration);
        break;
      case '4':
        tone(4, NOTE_D4,noteDuration);
        break;
      case '5':
        tone(4, NOTE_E4,noteDuration);
        break;
      case '6':
        tone(4, NOTE_F4,noteDuration);
        break;
      case '7':
        tone(4, NOTE_G4,noteDuration);
        break;
      case '8':
        tone(4, NOTE_A4,noteDuration);
        break;
      case '9':
        tone(4, NOTE_B4,noteDuration);
        break;
      case '0':
        tone(4, NOTE_C5,noteDuration);
        break;
      case '*':
//        digitalWrite(ledpin, LOW);     
//        tone(4, NOTE_G4,noteDuration);
        break;
      case '#':
//        digitalWrite(ledpin, HIGH);
        break;
      default:
        Serial.println(key);
    }
  }
}

