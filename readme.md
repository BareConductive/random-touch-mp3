[![Bare Conductive](http://bareconductive.com/assets/images/LOGO_256x106.png)](http://www.bareconductive.com/)

# Bare Conductive Random Touch MP3 Demo Code

Touch-to-MP3 code for the [Bare Conductive Touch Board](http://www.bareconductive.com/shop/touch-board/) playing a random track from fixed folders on the microSD card. 

You need twelve folders named **E0** to **E11** in the root of the microSD card. Each of these folders can have as many MP3 files in as you like, named however you 
like (as long as they have a .mp3 file extension).

When you touch electrode **E0**, a random file from the **E0** folder will play. When you touch electrode **E1**, a random file from the **E1** folder will play, and so on. You should note that this is not the same file structure as for [Touch_MP3](https://github.com/BareConductive/touch-mp3).

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
  
## Requirements
* You should make sure that you have followed our [Setting up Arduino with your Touch Board](http://www.bareconductive.com/make/setting-up-arduino-with-your-touch-board/) tutorial before using this (or any other) of our code examples


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
