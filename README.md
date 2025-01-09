# starmap-obe

starmap-obe is a star/planets engine that can be either set as an Arduino library, used on a raspberry Pi with a screen and a way to set time (internet connection or RTC), or executed on a Linux/macOS computer.
The project needs to be compiled to the correct platform.

| ![starmap-obe !](/img/starmap.png?raw=true) | 
|:--:| 
| *Twinkle, twinkle litte star ...* |

## Acknowledgments
Based on starmap : [shabaz123's starmap](https://github.com/shabaz123/starmap). This project has been largely based on storybook with a few tweaks for my needs. All credit goes to them for making this awesome project.

## Building library from source

If you want to build the library from source, you can clone the code into a folder onto your Linux machine, then go into the starmap folder, and type:

```
chmod 755 make_libraries.py
./make_libraries.py
```

The output will be two zip files, one folder up from the starmap folder. The zip file titled `starmap_library-1.0.0.zip` will be the bundled library that you can add into the Arduino development environment (using **Sketch->Include Library->Add .ZIP file**).

The other zip file that will be generated, called `starmap_lib.zip` is useful for CMake projects for non-Arduino platforms, such as Pi Pico C/C++ SDK. It can be ignored if you're using Arduino.

## starmap on Linux
If you wish to test the code on Linux, prior to uploading to any microcontroller, then you can do that by typing:
```
cd starmap-obe/examples/StarmapLinuxTest
mkdir -p build
cd build
cmake ..
make
```

By doing that, a `starmap` Linux executable will be built in the build folder. Note that you may need to install `libpng` on your Linux machine beforehand (on Ubuntu, you can type `sudo apt install libpng`). You can run the starmap executable by typing:

```
./starmap
```

It will generate a star chart PNG file (called out.png) which can be inspected.

## starmap on MacOS
If you wish to test the code on macOS, then you can do that by typing:
```
cd starmap-obe/examples/StarmapMacOS
mkdir -p build
cd build
cmake ..
make
```

By doing that, a `starmap` macOS executable will be built in the build folder. Note that you may need to install `libpng` on your MacOS machine beforehand (you can type `brew install libpng`). You can run the starmap executable by typing:

```
./starmap
```

It will generate a star chart PNG file (called out.png) which can be inspected.
```
open sky.png
```

## starmap on RPi Zero 2W + round 1.28'' screen + DS3231 RTC

### Required hardware
- Raspberry Pi Zero 2W [link](https://www.raspberrypi.com/products/raspberry-pi-zero-2-w/)
- 240×240, General 1.28inch Round LCD IPS Display Module, with GC9A01 driver. 
   - The Waveshare display has an onboard voltage regulator and can be used with either 3.3 or 5-volt logic, the power supply voltage should match the logic level (although you CAN use a 5-volt supply with 3.3-volt logic). [link](https://www.waveshare.com/1.28inch-lcd-module.htm).
   - Some alternatives have no onboard voltage regulator, so it is limited to working with 3.3-volt logic, and can be found on Aliexpress
   - The driver used is the GC9A01
- PiSugar3 for RpiZero [link](https://www.tindie.com/products/pisugar/pisugar-3-battery-for-raspberry-pi-zero/) or Waveshare UPS HAT (C) for RPi Zero [link](https://www.waveshare.com/wiki/UPS_HAT_(C)).
- DS3231 RTC module. Adafruit has a compact one [link](https://www.adafruit.com/product/3013), but any can do. *NB: PiSugar has an RTC embedded*.

###  Dependencies
The following dependencies need to be installed :
```
sudo apt update
sudo apt upgrade
sudo apt install cmake libpng-dev unzip
sudo apt install liblgpio-dev
sudo apt install python3-smbus i2c-tools # for DS3231
```

### Setting up
1. Image the SD card with RPi OS Bookworm 64bit lite, then boot and update the OS.
2. Set locale correctly using the following :
``` bash
locale #to see locales
sudo update-locale "LC_ALL=en_GB.UTF-8"
sudo update-locale "LANGUAGE=en_GB:en"
```
then reboot.

3. Go to `sudo raspi-config` and enable the following
 - `I2C` interface (for the DS3231 RTC)
 - `SPI` interface (for the screen)
 - [OPTIONAL] `ssh`
 - [OPTIONAL] set up wifi

4. Install required dependencies (see above)

5. Connect the hardware (screen and RTC) and setup RTC (see below)

6. Clone this repository. `git clone https://github.com/obenchekroun/starmap-obe.git`

### Building
```
cd starmap-obe/examples/StarmapRPiZero
mkdir -p build
cd build
cmake ..
make
```

By doing that, a `starmap`  executable will be built in the build folder. 

### Usage
You can run the starmap executable by typing:
```
./starmap
```

It will continously display the chart on the screen + the time. Default refresh of the sky image is 5 minutes, but can be changed in `starmap-obe/examples/StarmapRPiZero/main.cpp`, by changing the value of this definition `#define DEFAULT_UPDATE_DELAY 5 // in minutes`
It will also generate a star chart PNG file (called sky.png) everytime the sky is refreshed, which can be inspected

### Connecting the hardware
#### Connecting the screen

| LCD | BCM2835 (GPIO number) | Board (Pin) |
|-----|-----------------------|-------------|
| VCC |                  3.3V |        3.3V |
| GND |                   GND |         GND |
| DIN |                  MOSI |          19 |
| CLK |                  SCLK |          23 |
| CS  |                   CE0 |          24 |
| DS  |                    25 |          22 |
| RST |                    27 |          13 |
| BL  |                    18 |          12 |

*NB : BL is the connector for the backlight control. It is not present on some screen model*

| ![Screen connection](/img/screen_pinout.jpg) |
|:--:| 
| *screen connection diagram* |

#### Connecting and setting up the DS3231 RTC
1. Enable I2C : `sudo raspi-config` and enable i2c in Interface Options > I5 I2C then reboot `sudo reboot`
2. Install required libraries and tools : `sudo apt install python3-smbus i2c-tools`
3. Connect the DS3231, following this pining :

| DS3231  | RPi connection | RPi pin | 
| --- | --- | --- |
| VCC  | 3V3 | Pin 1 |
| GND  | GND | Pin 6 e.g  |
| SDA  | GPIO 5 | Pin 29  |
| SCL  | GPIO 6 | Pin 31  |
| INT/SQW  | Reset Pin | pin 5 (RPi zero 2W) or pin 3 (RPi 4)  |

| ![RPi Zero 2W Pin out diagram](/img/Zero2W3.jpg.webp) |
|:--:| 
| *RPi Zero 2W Pin out diagram* |

4. Edit /boot/config.txt by adding dtoverlay config and reboot : 
 * `sudo nano /boot/firmware/config.txt`
 * Add the following : 
 ```
 #dtoverlay for RTC DS3231 on specific pin
 dtoverlay=i2c-rtc-gpio,ds3231,i2c_gpio_sda=5,i2c_gpio_scl=6,wakeup-source
 ```
Make sure that the GPIO pin in the dtoverlay code corresponds to the pinning of the DS3231 to the RPi.
 * Then reboot `sudo reboot`

5. To detect and list i2c hardware :
``` sh
 sudo i2cdetect -l # list devices
 sudo i2cdetect -y 11 # (replace 11 with bus, usually 11 for this dtoverlay with i2c-rtc-gpio )
```
Output :

``` sh
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- 57 -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --   
```
68 is code of RTC clock
if UU appear, mean driver loaded. Otherwise, verify /boot/config.txt and reboot.

6. Now that we have successfully got the kernel driver activated for the RTC Chip and we know it’s communicating with the Raspberry Pi, we need to remove the “fake-hwclock“ package. This package acts as a placeholder for the real hardware clock when you don’t have one.

``` sh
sudo apt-get -y remove fake-hwclock
sudo update-rc.d -f fake-hwclock remove
sudo systemctl disable fake-hwclock
```

7. Now that we have disabled the “fake-hwclock” package we can proceed with getting the original hardware clock script that is included in Raspbian up and running again by commenting out a section of code.
Run the following command to begin editing the original RTC script.

``` sh
sudo nano /lib/udev/hwclock-set
```
and comment out : 

``` sh
#if [ -e /run/systemd/system ] ; then
# exit 0
#fi

```

8. Now, if we have to sync time of the RTC to the one of the RPi (obtained with internet connection) :

``` sh
sudo hwclock -r #get time from RTC clock
date #see if time is correct of RPi
sudo hwclock -w # write time to RTC
```

### Misc

#### Waveshare test library

You can use Waveshare test library to test the display. It is included in the `examples/screentest` folder :

``` sh
cd starmap/examples/screentest
mkdir -p build
cd build
cmake ..
make
```

and then test it :
``` sh
sudo ./screentest 1.28
```

#### Downloading from Waveshare website
You can also use the Waveshare test library to test the display bu downloading it from Waveshare website. Here are the instructions found in the [wiki](https://www.waveshare.com/wiki/1.28inch_LCD_Module) :

``` sh
sudo wget https://files.waveshare.com/upload/8/8d/LCD_Module_RPI_code.zip
sudo unzip ./LCD_Module_RPI_code.zip 
cd LCD_Module_RPI_code/RaspberryPi/
```

then build it : 
``` sh
cd c
sudo make clean
sudo make -j 8
```

 and test it for the corresponding display :
``` sh
#1.28inch LCD Module
sudo ./main 1.28
```


## Acknowledgement

Note: This code uses content from https://www.fourmilab.ch/homeplanet/ (public domain code).

See the screenshot here:

<img width="100%" align="left" src="screenshot-public-domain-code.jpg">


