[![PlatformIO](https://github.com/shabaz123/starmap/workflows/PlatformIO/badge.svg)](https://github.com/shabaz123/starmap/actions/) ![ViewCount](https://views.whatilearened.today/views/github/shabaz123/starmap.svg)

# starmap

The code in this repository is a star/planets engine in an Arduino library.

The ready-made Arduino library zip file can be found by clicking on **Releases**.

If you want to build the library from source, you can clone the code into a folder onto your Linux machine, then go into the starmap folder, and type:

```
chmod 755 make_libraries.py
./make_libraries.py
```

The output will be two zip files, one folder up from the starmap folder. The zip file titled **starmap_library-1.0.0.zip** will be the bundled library that you can add into the Arduino development environment (using **Sketch->Include Library->Add .ZIP file**).

The other zip file that will be generated, called **starmap_lib.zip** is useful for CMake projects for non-Arduino platforms, such as Pi Pico C/C++ SDK. It can be ignored if you're using Arduino.

If you wish to test the code on Linux, prior to uploading to any microcontroller, then you can do that by typing:

```
cd starmap/examples/StarmapLinuxTest
mkdir -p build
cd build
cmake ..
make
```

By doing that, a **starmap** Linux executable will be built in the build folder. Note that you may need to install **libpng** on your Linux machine beforehand (on Ubuntu, you can type **sudo apt install libpng**). You can run the starmap executable by typing:

```
./starmap
```

It will generate a star chart PNG file (called out.png) which can be inspected.

# starmap on MacOS
```
cd starmap/examples/StarmapMacOS
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
open out.png
```

# starmap on RPi Zero 2W + round 1.28'' screen + DS3231 RTC

## Required hardware
- Rpi Zero 2W
- 240×240, General 1.28inch Round LCD IPS Display Module, with GC9A01 driver. 
   - The Waveshare display has an onboard voltage regulator and can be used with either 3.3 or 5-volt logic, the power supply voltage should match the logic level (although you CAN use a 5-volt supply with 3.3-volt logic). [link](https://www.waveshare.com/1.28inch-lcd-module.htm).
   - Some alternatives have no onboard voltage regulator, so it is limited to working with 3.3-volt logic, and can be found on Aliexpress
   - The driver used is the GC9A01
- PiSugar3 for RpiZero [link](https://www.tindie.com/products/pisugar/pisugar-3-battery-for-raspberry-pi-zero/) or Waveshare UPS HAT (C) for RPi Zero [link](https://www.waveshare.com/wiki/UPS_HAT_(C)).
- DS3231 RTC module. Adafruit has a small one [link](https://www.adafruit.com/product/3013), but any can do.

## Installing dependencies

### Software dependencies
The following dependencies need to be installed :
```
sudo apt update
sudo apt upgrade
sudo apt install cmake libpng-dev unzip
# sudo apt install python3-setuptools # required for compiling lgpio
sudo apt install liblgpio-dev
```

### Setting up

1) Flash the Bookworm 64-bit lite image
2) Activate SPI (for the display) and I2C (for DS3231) in `sudo raspi-config` > `Interface Options`

## Building
```
cd starmap/examples/StarmapRPiZero
mkdir -p build
cd build
cmake ..
make
```

By doing that, a `starmap`  executable will be built in the build folder. You can run the starmap executable by typing:

```
./starmap
```

It will generate a star chart PNG file (called out.png) which can be inspected.

### Waveshare test library

You can use Waveshare test library to test the display. It can be downloaded with the following :
``` sh
sudo wget https://files.waveshare.com/upload/8/8d/LCD_Module_RPI_code.zip
sudo unzip ./LCD_Module_RPI_code.zip 
cd LCD_Module_RPI_code/RaspberryPi/
```

then built : 

``` sh
cd c
sudo make clean
sudo make -j 8
```

 and tested for the corresponding display :

``` sh
#1.28inch LCD Module
sudo ./main 1.28
```


## Acknowledgement

Note: This code uses content from https://www.fourmilab.ch/homeplanet/ (public domain code).

See the screenshot here:

<img width="100%" align="left" src="screenshot-public-domain-code.jpg">


