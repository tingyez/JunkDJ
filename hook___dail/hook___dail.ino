#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif

/**
 * \brief Object instancing the SdFat library.
 *
 * principal object for handling all SdCard functions.
 */
SdFat sd;

/**
 * \brief Object instancing the SFEMP3Shield library.
 *
 * principal object for handling all the attributes, members and functions for the library.
 */
SFEMP3Shield MP3player;
int16_t last_ms_char; // milliseconds of last recieved character from Serial port.
int8_t buffer_pos; // next position to recieve character from Serial port.

//------------------------------------------------------------------------------
/**
 * \brief Setup the Arduino Chip's feature for our use.
 *
 * After Arduino's kernel has booted initialize basic features for this
 * application, such as Serial port and MP3player objects with .begin.
 * Along with displaying the Help Menu.
 *
 * \note returned Error codes are typically passed up from MP3player.
 * Whicn in turns creates and initializes the SdCard objects.
 *
 * \see
 * \ref Error_Codes
 */
  char buffer[6]; // 0-35K+null


//////////////// BLUETOOTH
#define bt_power 7
#define bt_key_power 8
#define indication_led 13

#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3); // RX | TX

char c = ' ';
//////////////// END BLUETOOTH


// constants

int dialHasFinishedRotatingAfterMs = 100;
int debounceDelay = 10;
int needToPrint = 0;
int count;
int in = A1;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0; 
int cleared = 0;

void setup() {

  uint8_t result; //result code from some function as to be tested at later time.
  
  pinMode(in, INPUT);//dail

  Serial.begin(9600);
  Serial.println("Welcome to JunkDJ!");
  Serial.println();
  Serial.println();
  Serial.println("Please dail the phone to play music:");
  Serial.println("1-9: select a track");
  Serial.println("0: stop the music");
  Serial.println();
  Serial.println();


  ///////////////////////////////////////////////////// SETUP BLUETOOTH
  // set the pins to OUTPUT
  pinMode(bt_power, OUTPUT);  
  pinMode(bt_key_power, OUTPUT);
  pinMode(indication_led, OUTPUT);
  
  // set the pins to LOW
  digitalWrite(bt_power, LOW);
  digitalWrite(bt_key_power, LOW);
  digitalWrite(indication_led, LOW);

  // make sure the key has been LOW for a bit
  delay(100);
  
  // set the key pin to High
  digitalWrite(bt_key_power, HIGH);
  
  // small delay
  delay(100);
  
  // now power on the BT
  digitalWrite(bt_power, HIGH);
  
  // start our serial so we can send and recieve
  // information from the BT module
  Serial.begin(9600);
  // initiate the BT serial at 38400 which is the default 
  // speed at which the BT AT mode operates at
  BT.begin(38400);
  Serial.println("BT started at 38400");
  
  // self explanatory
  Serial.write("For a list of commands, visit: \n");
  Serial.write("Type AT commands  \n\n");
  
  // process complete turn on led 13
  digitalWrite(indication_led, HIGH);
  
  // Send an "AT" command to the AT (without quotes)
  // if response is OK, then we are connected
  // and ready to program the BT module
  
  ///////////////////////////////////////////////////// END SETUP BLUETOOTH
  
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  //check result, see readme for error codes.
  }

  #if (0)
  // Typically not used by most shields, hence commented out.
  //Serial.println(F("Applying ADMixer patch."));
  if(MP3player.ADMixerLoad("admxster.053") == 0) {
    //Serial.println(F("Setting ADMixer Volume."));
    MP3player.ADMixerVol(-3);
  }
  #endif

void loop()
{
  ///////////////////////////////////////// BLUETOOTH
  // listen for a response from the HC-05 and write it to the serial monitor
  if (BT.available()){
    c = BT.read();
    Serial.write(c);
  }
  // listen for user input and send it to the HC-05
  if (Serial.available()){
    c = Serial.read();
    Serial.write(c);
    BT.write(c);
  }
  ///////////////////////////////////////// END BLUETOOTH
  
  if (digitalRead(A0)==HIGH)
  {
    int reading = digitalRead(in);
  
  
    if ((millis() - lastStateChangeTime) > dialHasFinishedRotatingAfterMs) {
    // the dial isn't being dialed, or has just finished being dialed.
      if (needToPrint) {
      // if it's only just finished being dialed, we need to send the number down the serial
      // line and reset the count. We mod the count by 10 because '0' will send 10 pulses.
      
      if(count==1){
        MP3player.stopTrack();
        Serial.println("Dail 1-9 to select one track.");
        Serial.println();
      }
        
      else{
        Serial.print("You select track: ");
        Serial.println(count-1);
        Serial.println("The music is playing");
        MP3player.stopTrack();
        MP3player.playTrack(count-1);
        Serial.println();
        Serial.println("Hey,you can add some sound effect!");
        Serial.println();
      }
    
    
      needToPrint = 0;
      count = 0;
      cleared = 0;
      }
    } 
  
    if (reading != lastState) {
      lastStateChangeTime = millis();
    }
      if ((millis() - lastStateChangeTime) > debounceDelay) {
      // debounce - this happens once it's stablized
        if (reading != trueState) {
        // this means that the switch has either just gone from closed->open or vice versa.
        trueState = reading;
          if (trueState == HIGH) {
          // increment the count of pulses if it's gone high.
          count++; 
          needToPrint = 1; // we'll need to print this number (once the dial has finished rotating)
          } 
        }
      }
      lastState = reading;
  }
  else {
    MP3player.stopTrack();
  }
} 

