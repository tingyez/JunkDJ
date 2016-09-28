/* Basic Digital Read and Rotary Phone Dial Reader
 * ------------------ 
 * This code reads whether the phone is on the hook by treating that hook like a button that is either open or depressed
AND it reads out the number dialed on a rotary phone dial by counting the pulses made by the spinning dial wheel. 
 
 
 */
 
 
#define INITPIN 12
#define NUMPIN 11

int counter; // holds the pulse count for each dial spin
int currentValue = 0; 
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 5;    // the debounce time; increase if the output flickers
 
int inPin = 7;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status



void setup(){
   Serial.begin(9600);
  
  //configure the dial reading inputs 
  pinMode(INITPIN, INPUT_PULLUP);
  pinMode(NUMPIN, INPUT_PULLUP);
   
 
 pinMode(inPin, INPUT);    // declare phone hook as input
}

void loop(){  
 
 
    do { stopOperator(); } //when the phone is on the hook, don't play the operator's voice (or the dial tone)
      while (digitalRead(inPin) == HIGH ); // wait for phone to be picked up for the first time
      delay(100); // wait 100 milliseconds
   Serial.println("play operator"); // tell processing to turn on operator voice
   
   do { if (digitalRead (INITPIN) == LOW) { //if the wheel starts turning
                 stopOperator();  //stop the operator voice
                 readNumber(); //read and react to the numbers
                 }
                 
   } while (digitalRead(inPin) == LOW); //while the phone is off the hook
 delay(100); //   chill for a sec


}

void stopOperator(){
  Serial.println("stop operator"); 
}

void endRecording(){
  Serial.println("stop recording");
}


void readNumber(){
  int initRead = digitalRead (INITPIN); // Is the wheel turning or not?
  static int lastValue = HIGH;  // holds the last read from the pulse pin.

  if (initRead == LOW) {  // If the wheel is turning....

    int newValue = digitalRead (NUMPIN); // check the pulse pin.
    if (newValue != lastValue) { // if it's CHANGED from the last read...
      lastDebounceTime = millis(); // save its clock cycle; we need to check it.
    }
    // If enough time has passed (aka, it's not just a "bounce" from the 
    // analog signal)...
    if ((millis() - lastDebounceTime) > debounceDelay) { 
      // and if the current value is DIFFERENT than the one you just read...
      if (currentValue != newValue) { 
        currentValue = newValue; // make them the same.
        if (newValue == 1) { // If you just set it to a 1...
          counter++; // it just finished a pulse, so add one to the counter.
        }
      }
    }

    lastValue = newValue; // Your new value becomes the old one for comparison.

  } else {
// once the dial returns home and switches the initializing pin OFF,
// you can be sure that there will be no more pulses coming.
// "Counter" is the number dialed. You may now use it for whatever you want.
// This is adjusted for the special case of "0" actually being 10 pulses.
  
    if (counter > 0 && val!=HIGH) {
      if (counter == 10) {
      Serial.println (0);
      } 
      
      else if(counter == 6) {      // if the number is six
            Serial.println("play response");             // play something
         }
      else if (counter == 7) {    // if the number is seven
            Serial.println("record"); //record
      
      }
      else {
        Serial.println (counter);
      }     
      
// After you're done using it, reset counter so it can start again on the
// next dial.
  counter = 0;   
  }
 
}
}
