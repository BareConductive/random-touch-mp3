[![Bare Conductive](https://www.dropbox.com/s/p0p71yid2t8tenp/LOGO_256x106.png?dl=1)](http://www.bareconductive.com/)

# Bare Conductive Random Touch MP3 Demo Code

Touch-to-MP3 code for the [Bare Conductive Touch Board](http://www.bareconductive.com/shop/touch-board/) playing a random track from fixed folders on the micro SD card. Touch electrode 0, a random MP3 from folder **E0** on the micro SD card plays. Touch electrode 1, a random MP3 from folder **E1** plays from the micro SD card, and so on. The naming of the MP3 files does not matter, provided that they have the .mp3 extension and are placed in folders named **E0** to **E11**.

## Requirements
* [Arduino](http://arduino.cc/en/Main/Software) 1.5.6 or later

* Arduino's instructions for installing libraries are available [here](http://arduino.cc/en/Guide/Libraries)

* [SFEMP3shield library](https://github.com/madsci1016/Sparkfun-MP3-Player-Shield-Arduino-Library) (zip file download [here](https://github.com/madsci1016/Sparkfun-MP3-Player-Shield-Arduino-Library/archive/master.zip)) 	
* [Bare Conductive MPR121 library](https://github.com/bareconductive/mpr121) (zip file download [here](https://github.com/bareconductive/mpr121/archive/public.zip))

* [Bare Conductive Arduino Hardware Plugins](https://github.com/bareconductive/bare-conductive-arduino) (zip file download [here](https://github.com/bareconductive/bare-conductive-arduino/archive/public.zip)) - instructions on installing this hardware plugin folder are available [here](https://github.com/bareconductive/bare-conductive-arduino).


## Install

1. Close the Arduino IDE if you have it open.
1. Download the [.zip](https://github.com/BareConductive/random-touch-mp3/archive/public.zip) or clone the repository to your local machine - if downloading the .zip, extract the contents somewhere that suits you.
1. Take the **Random_Touch_MP3** folder and move it to **Arduino Sketchbook Folder**. This will be different for each operating system: 

	**Windows**
	
	Libraries\\Documents\\Arduino
	
	or
	
	My Documents\\Arduino	
	
	**Mac**
	
	Documents/Arduino
	
	**Linux (Ubuntu)**
	
	Home/Arduino


	If this folder does not exist, create it first.
1. Reopen the Arduino IDE - you should now be able to open the sketch in the **File -> Sketchbook** menu.