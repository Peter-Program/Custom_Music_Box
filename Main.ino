// Magic Music Box
/*
 * This is a program for the arduino micro. The goal is to play a random sound 
 * file when the lid of the chest is opened. It can stop playing the sound file
 * when the lid is closed. Additionaly, durring a random interval based on
 * NaviBase, a sound from Zelda OoT character navi will play. There exists a
 * day/night cycle that starts once the system is powered and the day/night 
 * status will change after the specified time has passed of DayIntervalBase in
 * hours.
 */
// Version 2.2

#include <SD.h>
#include <pcmConfig.h>
#include <pcmRF.h>
#include <TMRpcm.h>
#include <SPI.h>

#define SD_ChipSelectPin 4
#define SpeakerInputPin 9                                                       // Pin number for arduino to speaker
#define ChestButtonPin 2                                                        // Pin number for arduino to chest lid switch
#define SystemVolume 5                                                          // volume level for system (5 results in the best quality)
//#define NaviBase 1                                                              // Number of minuets for the base interval for navi sound occurrences minimum minuet to wait
//#define DayIntervalBase 12                                                      // Number of hours for the day and night cycle to occur 

#define NAVISOUNDSSIZE 7                                                        // The length of the array naviSounds
#define SOUNDFILESIZE 132



/* SD card reader to Arduino
 * MOSI – pin 11 on Arduino Uno/Duemilanove/Diecimila
 * MISO – pin 12 on Arduino Uno/Duemilanove/Diecimila
 * SCK – pin 13 on Arduino Uno/Duemilanove/Diecimila
 * CS –  Pin 4 (SD_ChipSelectPin) depends on your SD card shield or module.
 * VCC 5v
 * GND
*/

/* Audio Amp
 * Vcc 5v to 12v
 * In - speakerPin 9 from arduino
 * GND - ground
 * GND - ground
 * From green terminal plug in speaker 
 */
// https://github.com/TMRh20/TMRpcm/wiki
// TMRpcm audio;
// audio.play("filename");    plays a file
// audio.play("filename",30); plays a file starting at 30 seconds into the track
// audio.speakerPin = 11;     set to 5,6,11 or 46 for Mega, 9 for Uno, Nano, etc.
// audio.disable();           disables the timer on output pin and stops the music
// audio.stopPlayback();      stops the music, but leaves the timer running
// audio.isPlaying();         returns 1 if music playing, 0 if not
// audio.pause();             pauses/unpauses playback
// audio.quality(1);          Set 1 for 2x oversampling
// audio.volume(0);           1(up) or 0(down) to control volume
// audio.setVolume(0);        0 to 7. Set volume level
// audio.loop(1);             0 or 1. Can be changed during playback for full control of looping. 

/*------------------------------------------------------------------------------
 *                              Global Variables
 *----------------------------------------------------------------------------*/


 // Navi -----------------------------------------------------------------------
 // See "Song Titles.txt" file in the 8-bit folder to identify the songs
char *naviSounds[] = {
  "N001.wav",                                                                   // Hello
  "N002.wav",                                                                   // Hey
  "N003.wav",                                                                   // Listen
  "N004.wav",                                                                   // Look
  "N005.wav",                                                                   // Watchout
  "N006.wav",                                                                   // Bonk
  "N007.wav"                                                                    // Float
};


 // Pin Assignments ------------------------------------------------------------
  const int chestButton = ChestButtonPin;

 // Other ----------------------------------------------------------------------
 TMRpcm file;
 unsigned long currTime;
 boolean chestStateClosed = false;
 boolean rndFilePlaying = false;
 int naviSoundsSize;

 // Day/Night-------------------------
 boolean timeDay = true;                                                        // true if it is day false if it is night
 unsigned long prevDay = 0;                                                     // Previous Day Triggered Time
 unsigned long dayInterval = 43200000;                  // hours in milliseconds (mill*sec*min*HOUR)


 //Navi-------------------------------
  unsigned long prevNavi = 0;                                                   // Previous Navi Triggered time 
  unsigned long baseNaviInterval = 60000;                                       // Base interval of navi sound occurrences Mili*Sec*Min (1 minuite)
  unsigned long naviInterval = 60000;                                           // The interval of navi that gets multiplied by a rnd mult and the base
  boolean naviOut = false;                                                      // Used To rmember if Navi is 'in' or 'out' of the chest

 /*-----------------------------------------------------------------------------
 *                              End Global Variables
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *                                   Setup
 *----------------------------------------------------------------------------*/
 
void setup() {

  pinMode(chestButton, INPUT);                                                  // Establish chestButton
  file.speakerPin = SpeakerInputPin;                                            // Establish what pin the speaker is connected to

  Serial.begin(9600);                                                           // Serial debug output

  if (!SD.begin(SD_ChipSelectPin)) {                                            // Checking if we can read from SD card, Stop if CANNOT read SD
    Serial.println("SD fail");
    return;
  }

  Serial.println("OK");
  file.setVolume(SystemVolume);                                                 // Setting Speaker pin Vol to 5 for best results
  // need to test quality function to see if I need it
  //file.quality(1);                                                              // Improve audio quality 2x oversampiling

  naviMultInterval();                                                           // Sets Navi Interval

  // Get lengths of arrays
  naviSoundsSize = NAVISOUNDSSIZE;

  randomSeed(analogRead(0));
}


/*------------------------------------------------------------------------------
 *                                     Loop
 *----------------------------------------------------------------------------*/
void loop() { 
  currTime = millis();                                                          // Getting Current Time of system

  waitForSongToFinish();                                                        // Make sure nothing is currently being played before continuing

  Serial.println("Main Loop");
  playNaviSound();
  //dayOrNight();

  if (digitalRead(chestButton) == HIGH) {                                       // Chest is closed
      Serial.println("Chest is now Closed");
      chestStateClosed = true;                                                  // Setting state of chest lid to closed
      rndFilePlaying = false;                                                   // Setting to false since no rndFile should be playing at this time
  }

  if(chestOpenOrClosed(chestButton)) {                                          // if Chest has just been opened then play a random file
      Serial.println("Chest just got opened");
      playRandomFile();
  }

  delay(500);                                                                   // delay to not put strain on system

} 