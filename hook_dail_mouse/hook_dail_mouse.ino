#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

//-------------for the MOUSE
#define MDATA A2 //yellow
#define MCLK A3  //white


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
int in = 5;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0; 
int cleared = 0;
int countx, county, countz;

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


  //Initialize the MOUSE

  countx = 0;
  county = 0;
  countz = 0;
  
  mouse_init();
  
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
  while (digitalRead(10)==HIGH){
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
           Serial.println("Hey,you can add some sound effect!/n");
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

  mouse_ctrl();
  delay(20);
  
  }
} 

//----------------Function for the Mouse-----------------

void mouse_ctrl(){
  char mstat;
  char mx;
  char my;
  char mz;

  /* get a reading from the mouse */
  mouse_write(0xeb);  /* give me data! */
  mouse_read();      /* ignore ack */
  mstat = mouse_read();
  mx = mouse_read();
  my = mouse_read();
  mz = mouse_read();

  String xacc = String(mx, DEC);
  String yacc = String(my,DEC);
  String zacc = String(mz, DEC);

  if ( (mx != 0) || (my != 0 ) || (mz != 0 )){
    /* send the data back up */
    Serial.print(mstat, BIN);
    Serial.print("\tX=");    Serial.print(xacc);
    Serial.print("\tY=");    Serial.print(yacc);
    Serial.print("\tZ=");    Serial.print(zacc);
    Serial.println();  
  }
  
//  VOLUMN CTRL
  if (zacc < "0") {    // volumn UP
    Serial.println("now the music is LOUDER");
    
  }
  else if (zacc > "0"){    // volumn DOWN
    Serial.println("now the music is QUIETER");
    
  }

//  FREQUENCY CTRL

  if (xacc == "0"){
    countx = 0;
  }
  else if (xacc > "0"){
    ++countx;

    if (countx == 3){   // frequency UP
      Serial.println("now the music is FASTER");

      countx = 0;
    }
  }
  else if (xacc < "0"){
    --countx;

    if (countx == -3){    // frequency DOWN
      Serial.println("now the music is SLOWER");

      countx = 0;
    }
  }
}

void gohi(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void golo(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void mouse_write(char data)
{
  char i;
  char parity = 1;

//    Serial.print("Sending ");
//    Serial.print(data, HEX);
//    Serial.print(" to mouse\n");
//    Serial.print("RTS\n");
  /* put pins in output mode */
  gohi(MDATA);
  gohi(MCLK);
  delayMicroseconds(300);
  golo(MCLK);
  delayMicroseconds(300);
  golo(MDATA);
  delayMicroseconds(10);
  /* start bit */
  gohi(MCLK);
  /* wait for mouse to take control of clock); */
  while (digitalRead(MCLK) == HIGH)
    ;
  /* clock is low, and we are clear to send data */
  for (i=0; i < 8; i++) {
    if (data & 0x01) {
      gohi(MDATA);
    } 
    else {
      golo(MDATA);
    }
    /* wait for clock cycle */
    while (digitalRead(MCLK) == LOW)
      ;
    while (digitalRead(MCLK) == HIGH)
      ;
    parity = parity ^ (data & 0x01);
    data = data >> 1;
  }  
  /* parity */
  if (parity) {
    gohi(MDATA);
  } 
  else {
    golo(MDATA);
  }
  while (digitalRead(MCLK) == LOW)
    ;
  while (digitalRead(MCLK) == HIGH)
    ;
  /* stop bit */
  gohi(MDATA);
  delayMicroseconds(50);
  while (digitalRead(MCLK) == HIGH)
    ;

  gohi(MDATA);
    
  /* wait for mouse to switch modes */
  while ((digitalRead(MCLK) == LOW) || (digitalRead(MDATA) == LOW))
    ;
    
  /* put a hold on the incoming data. */
  golo(MCLK);
//    Serial.print("done.\n");
}

/*
 * Get a byte of data from the mouse
 */
char mouse_read(void)
{
  char data = 0x00;
  int i;
  char bit = 0x01;

  //  Serial.print("reading byte from mouse\n");
  /* start the clock */
  gohi(MCLK);
  gohi(MDATA);
  delayMicroseconds(50);
  while (digitalRead(MCLK) == HIGH)
    ;
  delayMicroseconds(5);  /* not sure why */
  while (digitalRead(MCLK) == LOW) /* eat start bit */
    ;
  for (i=0; i < 8; i++) {
    while (digitalRead(MCLK) == HIGH)
      ;
    if (digitalRead(MDATA) == HIGH) {
      data = data | bit;
    }
    while (digitalRead(MCLK) == LOW)
      ;
    bit = bit << 1;
  }
  /* eat parity bit, which we ignore */
  while (digitalRead(MCLK) == HIGH)
    ;
  while (digitalRead(MCLK) == LOW)
    ;
  /* eat stop bit */
  while (digitalRead(MCLK) == HIGH)
    ;
  while (digitalRead(MCLK) == LOW)
    ;
  /* put a hold on the incoming data. */
  golo(MCLK);
  //  Serial.print("Recvd data ");
  //  Serial.print(data, HEX);
  //  Serial.print(" from mouse\n");
  return data;
}

void mouse_init()
{ char mouseId;
   
  gohi(MCLK);
  gohi(MDATA);
//    Serial.println("Sending reset to mouse\n");
  mouse_write(0xff);
//    Serial.println("mouse_write");
  mouse_read();  /* ack byte */
//    Serial.print("Read ack byte1\n");
  mouse_read();  /* blank */
  mouse_read();  /* blank */
//    Serial.print("Setting sample rate 200\n");
  mouse_write(0xf3);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0xC8);  /* Set rate command */
  mouse_read();  /* ack */
//    Serial.print("Setting sample rate 100\n");
  mouse_write(0xf3);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0x64);  /* Set rate command */
  mouse_read();  /* ack */
//    Serial.print("Setting sample rate 80\n");
  mouse_write(0xf3);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0x50);  /* Set rate command */
  mouse_read();  /* ack */
//    Serial.print("Read device type\n");
  mouse_write(0xf2);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_read();  /* mouse id, if this value is 0x00 mouse is standard, if it is 0x03 mouse is Intellimouse */
//    Serial.print("Setting wheel\n");
  mouse_write(0xe8);  /* Set wheel resolution */
  mouse_read();  /* ack */
  mouse_write(0x03);  /* 8 counts per mm */
  mouse_read();  /* ack */
  mouse_write(0xe6);  /* scaling 1:1 */
  mouse_read();  /* ack */
  mouse_write(0xf3);  /* Set sample rate */
  mouse_read();  /* ack */
  mouse_write(0x28);  /* Set sample rate */
  mouse_read();  /* ack */
  mouse_write(0xf4);  /* Enable device */
  mouse_read();  /* ack */

//    Serial.print("Sending remote mode code\n");
  mouse_write(0xf0);  /* remote mode */
  mouse_read();  /* ack */
//    Serial.print("Read ack byte2\n");
  delayMicroseconds(100);
}
