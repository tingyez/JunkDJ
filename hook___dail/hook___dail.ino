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
else
{
MP3player.stopTrack();
}
} 

