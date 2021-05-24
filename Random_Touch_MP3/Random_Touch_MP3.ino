
/*******************************************************************************

 Bare Conductive Random Touch MP3 player
 ---------------------------------------

 Random_Touch_MP3.ino - touch triggered MP3 playback taken randomly from microSD

 You need twelve folders named E0 to E11 in the root of the microSD card. Each
 of these folders can have as many MP3 files in as you like, named however you
 like (as long as they have a .mp3 file extension).

 When you touch electrode E0, a random file from the E0 folder will play. When
 you touch electrode E1, a random file from the E1 folder will play, and so on.
 You should note that this is not the same file structure as for Touch_MP3.

  SD card
  │
  ├──E0
  │    some_mp3.mp3
  │    another_mp3.mp3
  │
  ├──E1
  │    dog-barking-1.mp3
  │    dog-barking-2.mp3
  │    dog-growling.mp3
  │    dog-howling.mp3
  │
  ├──E2
  │    1.mp3
  │    2.mp3
  │    3.mp3
  │    4.mp3
  │    5.mp3
  │    6.mp3
  │
  └──...and so on for other electrodes

 Based on code by Jim Lindblom and plenty of inspiration from the Freescale
 Semiconductor datasheets and application notes.

 Bare Conductive code written by Stefan Dzisiewski-Smith and Peter Krige.

 This work is licensed under a MIT license https://opensource.org/licenses/MIT

 Copyright (c) 2016, Bare Conductive

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*******************************************************************************/

// compiler error handling
#include "Compiler_Errors.h"

// touch includes
#include <MPR121.h>
#include <MPR121_Datastream.h>
#include <Wire.h>

// MP3 includes
#include <SPI.h>
#include <SdFat.h>
#include <FreeStack.h>
#include <SFEMP3Shield.h>

// touch constants
const uint32_t BAUD_RATE = 115200;
const uint8_t MPR121_ADDR = 0x5C;
const uint8_t MPR121_INT = 4;

// serial monitor behaviour constants
const bool WAIT_FOR_SERIAL = false;

// MPR121 datastream behaviour constants
const bool MPR121_DATASTREAM_ENABLE = false;

// MP3 variables
SFEMP3Shield MP3player;
byte result;
int lastPlayed = 0;

// sd card instantiation
SdFat sd;
SdFile file;

// define LED_BUILTIN for older versions of Arduino
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);

  if (WAIT_FOR_SERIAL) {
    while (!Serial);
  }

  // initialise the Arduino pseudo-random number generator with
  // a bit of noise for extra randomness - this is good general practice
  randomSeed(analogRead(0));

  if (!sd.begin(SD_SEL, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

  if (!MPR121.begin(MPR121_ADDR)) {
    Serial.println("error setting up MPR121");
    switch (MPR121.getError()) {
      case NO_ERROR:
        Serial.println("no error");
        break;
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;
    }
    while (1);
  }

  MPR121.setInterruptPin(MPR121_INT);

  if (MPR121_DATASTREAM_ENABLE) {
    MPR121.restoreSavedThresholds();
    MPR121_Datastream.begin(&Serial);
  } else {
    MPR121.setTouchThreshold(40);
    MPR121.setReleaseThreshold(20);
  }

  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);  // reasonable for larger capacitances

  digitalWrite(LED_BUILTIN, HIGH);  // switch on user LED while auto calibrating electrodes
  delay(1000);
  MPR121.autoSetElectrodes();  // autoset all electrode settings
  digitalWrite(LED_BUILTIN, LOW);

  result = MP3player.begin();
  MP3player.setVolume(10, 10);

  if (result != 0) {
    Serial.print("Error code: ");
    Serial.print(result);
    Serial.println(" when trying to start MP3 player");
  }
}

void loop() {
  MPR121.updateAll();

  if (MPR121.getNumTouches() <= 1) {
    for (int i=0; i < 12; i++) {  // Check which electrodes were pressed
      if (MPR121.isNewTouch(i)) {
            if (!MPR121_DATASTREAM_ENABLE) {
              Serial.print("pin ");
              Serial.print(i);
              Serial.println(" was just touched");
            }

            digitalWrite(LED_BUILTIN, HIGH);

            if (i <= 11 && i >= 0) {
              if (MP3player.isPlaying()) {
                if (lastPlayed == i) {
                  // if we're already playing from the requested folder, stop it
                  MP3player.stopTrack();

                  if (!MPR121_DATASTREAM_ENABLE) {
                    Serial.print("stopping track ");
                    Serial.println(i-0);
                  }
                }
              } else {
                // if we're already playing a different track, stop that
                // one and play the newly requested one
                MP3player.stopTrack();

                if (!MPR121_DATASTREAM_ENABLE) {
                  Serial.print("playing track ");
                  Serial.println(i-0);
                }

                playRandomTrack(i);

                // don't forget to update lastPlayed - without it we don't have a history
                lastPlayed = i;
              }
            } else {
              // if we're playing nothing, play the requested track and update lastplayed
              playRandomTrack(i);
              lastPlayed = i;
            }
        } else {
          if (MPR121.isNewRelease(i)) {
            if (!MPR121_DATASTREAM_ENABLE) {
              Serial.print("pin ");
              Serial.print(i);
              Serial.println(" is no longer being touched");
            }

            digitalWrite(LED_BUILTIN, LOW);
          }
        }
    }
  }

  if (MPR121_DATASTREAM_ENABLE) {
    MPR121_Datastream.update();
  }
}

void playRandomTrack(int electrode) {
  // build our directory name from the electrode
  char thisFilename[255];  // 255 is the longest possible file name size
  // start with "E00" as a placeholder
  char thisDirname[] = "E00";

  if (electrode < 10) {
    // if <10, replace first digit...
    thisDirname[1] = electrode + '0';
    // ...and add a null terminating character
    thisDirname[2] = 0;
  } else {
    // otherwise replace both digits and use the null
    // implicitly created in the original declaration
    thisDirname[1] = (electrode/10) + '0';
    thisDirname[2] = (electrode%10) + '0';
  }

  sd.chdir();  // set working directory back to root (in case we were anywhere else
    if (!sd.chdir(thisDirname)) {  // select our directory
    Serial.println("error selecting directory");  // error message if reqd.
  }

  size_t filenameLen;
  char* matchPtr1;
  char* matchPtr2;
  unsigned int numMP3files = 0;

  // we're going to look for and count
  // the MP3 files in our target directory
  while (file.openNext(sd.vwd(), O_READ)) {
    file.getName(thisFilename, sizeof(thisFilename));
    file.close();

    filenameLen = strlen(thisFilename);
    matchPtr1 = strstr(thisFilename, ".mp3");
    matchPtr2 = strstr(thisFilename, "._");
    // basically, if the filename ends in .MP3, we increment our MP3 count
    if (matchPtr1-thisFilename == filenameLen-4 && matchPtr2-thisFilename != 0) numMP3files++;
  }

  // generate a random number, representing the file we will play
  unsigned int chosenFile = random(numMP3files);

  // loop through files again - it's repetitive, but saves
  // the RAM we would need to save all the filenames for subsequent access
  unsigned int fileCtr = 0;

  sd.chdir();  // set working directory back to root (to reset the file crawler below)
  if (!sd.chdir(thisDirname)) {  // select our directory (again)
    Serial.println("error selecting directory");  // error message if reqd.
  }

  while (file.openNext(sd.vwd(), O_READ)) {
    file.getName(thisFilename, sizeof(thisFilename));
    file.close();

    filenameLen = strlen(thisFilename);
    matchPtr1 = strstr(thisFilename, ".mp3");
    matchPtr2 = strstr(thisFilename, "._");
    // this time, if we find an MP3 file...
    if (matchPtr1-thisFilename == filenameLen-4 && matchPtr2-thisFilename != 0) {
      // ...we check if it's the one we want, and if so play it...
      if (fileCtr == chosenFile) {
        // this only works because we're in the correct directory
        // (via sd.chdir() and only because sd is shared with the MP3 player)
        if (!MPR121_DATASTREAM_ENABLE) {
          Serial.print("playing track ");
          Serial.println(thisFilename);  // should update this for long file names
        }
        MP3player.playMP3(thisFilename);
        return;
      } else {
        // ...otherwise we increment our counter
          fileCtr++;
      }
    }
  }
}
