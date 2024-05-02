# Volvo RTI Retrofit
Volvo P2 RTI retrofit with Android Auto, Carplay, Handsfree etc.

Version r1.0

Welcome to the OpenAutoPro project tailored for the Volvo P2! This project enhances your driving experience by integrating a Raspberry Pi and Arduino into your vehicle, replacing the Control Module from the RTI system. Utilizing the stock RTI display and controls ensures seamless integration.

RTI Retrofit - Android Auto
![RTI Retrofit - Android Auto](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/INT2.jpg)

# Special Thanks

I would like to extend my sincere thanks to the following contributors who have greatly contributed to the development and success of this project:
- **Karl Hagström**: His comprehensive guide on adding AUX functionality to the Volvo HU-XXXX radio has been invaluable. Check out the guide [here](https://gizmosnack.blogspot.com/2015/11/aux-in-volvo-hu-xxxx-radio.html).
- **klalle**: For updated AUX script and contributions. Check out their GitHub gist [here](https://gist.github.com/klalle/1ae1bfec5e2506918a3f89492180565e).
- **laurynas**: For developing the Volvo LINbus reader program, which played a crucial role in integrating SWM buttons into the OpenAutoPro system. Find the program [here](https://github.com/laurynas/volvo_linbus).
- **Luuk**: Their resource on enabling Android Auto on Volvo RTI systems has been immensely helpful. More details can be found [here](https://luuk.cc/p/vD2f/Android_Auto_on_Volvo_RTI).
- **BlueWaveStudio Team** For developing OpenAutoPro. More details can be found [here](https://bluewavestudio.io/).

Your contributions have significantly enriched this project, and I deeply appreciate your efforts. Thank you!

## What is now working:
- OpenAuto Pro with Bluetooth and internal audio player, hands-free calling, 15 band equalizer, etc.
- Android Auto Wireless
- Original display via serial video
- Opening and closing the display using buttons Enter+Back.
- Changing the display brightness, using buttons Next+Yes/No.
- The buttons on the steering wheel, including RTI buttons.

## Future works:
- Add the rear camera
- Add OBD info
- Add a small battery to maintain power during engine startup

## Table of Contents
1. [Connection Overview](#connection-overview)
   - [Necessary Components](#necessary-components)
   - [Cable Management](#cable-management)
   - [SWM Button Configuration - LIN bus](#swm-button-configuration---lin-bus)
   - [RTI Control Module Setup](#rti-control-module-setup)
   - [AUX Integration](#aux-integration)
2. [Programming Details](#programming-details)
   - [Arduino 1: Handling button inputs via LIN bus](#arduino-1-handling-button-inputs-via-lin-bus-controlling-the-rti-display)
   - [Arduino 2: Emulating a CD changer for AUX functionality](#arduino-2-emulating-a-cd-changer-for-aux-functionality)
   - [Raspberry Pi: OpenAutoPro installation, setup, Arduino connectivity, and configuring the splash screen](#raspberry-pi-openautopro-installation-setup-arduino-connectivity-and-configuring-the-splash-screen)

## Connection Overview

### Necessary Components
- Raspberry Pi 4, 2 GB RAM: SD Card 64GB
- [USB Sound Card Vention](https://www.alza.sk/vention-usb-external-sound-card-0-15m-gray-metal-type-omtp-ctia-d6093937.htm)
- Bluetooth [Orico BTA-409](https://www.alza.sk/orico-bta-409-biely-d6447277.htm)
- Arduino Nano (Original ATmega328) - Clone doesn’t read SWM buttons
- Arduino Nano (China clone) - For AUX
- MCP2025 Chip
- Step-Down Converter 12V-5V, 5A
- Ground Loop Isolator - From [Aliexpress](https://shorturl.at/lSVW0)
- Various cables, connectors, etc.

RTI Retrofit - OpenAutoPro
![RTI Retrofit - OpenAutoPro](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/INT1.jpg)

### Cable Management
Efficient cable management is essential for organizing the components of the OpenAutoPro project within the Volvo S60 D5. Let's discuss how we've optimized cable routing for a tidy and functional setup.
Utilizing Original Cables
- To maintain a clean and integrated installation, I've repurposed existing cables from the front of the car to the trunk.
- Two cables run from the RTI display and radio to the RTI control module. While one cable remains connected between the radio and the control module, we'll utilize the other 10-pin cable from the RTI display to the control module as an extension cord.
Power Source Selection
- To ensure seamless power management, we've tapped into the backside of the 12V socket in the front. We will use 2 pins for Power and ground in RTI cable.
- The 12V socket offers the advantage of cutting off power entirely when the ignition is switched off, reducing the risk of power drain.
- In future iterations, I plan to incorporate a small battery to maintain power during engine startup, further enhancing system reliability.

![RTI Controll module](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/RTI-box1.jpg)
![RTI Controll module Disassembley](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/RTI-box2.jpg)
![RTI Controll module Disassembled](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/RTI-box3.jpg)

### Display Cable Connection:
Incorporating the RTI (Road and Traffic Information) display into the OpenAutoPro system requires a meticulous approach to cable connection. Let's break down the cable setup for seamless integration.
Cable Components:
- Video Cable: This cable carries the video signal from the source to the RTI display, facilitating visual output.
- Ground Cable: Ensures proper grounding for stable operation and electrical safety.
- Display Serial Cable: Facilitates communication between the display and the system, enabling control and data exchange.

![RTI Display Pinout](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/rti-disp.png)

### SWM Button Configuration - LIN bus
The SWM buttons from the steering wheel, along with the RTI buttons, will be read through the LIN bus. We will utilize the original Arduino Nano with MCP2025 and the "laurynas" Volvo LINbus reader program (https://github.com/laurynas/volvo_linbus). Simply pressing the required button is sufficient to view the LIN bus key code from our buttons, without the need to turn on the ignition. The key code for the Volvo S60 2008 appears as follows: Enter button: “ C1 3F 20 A0 ”

The cables from the SWM/SAS are located up to the steering wheel, under the cover. See blue arrow.
![SWM Location](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/SWM%20Location.png)
![SWM Pinout](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/SWM%20Pins.png)

### RTI Control Module Setup
I have removed almost everything from the RTI Control Module box. I only left two connectors: an 8-pin DIN socket and a 10-pin connector from the display. I have placed all the computing components in this box: Raspberry Pi with all its components, a step-down converter, and two Arduino microcontrollers on a PCB.

RTI New Setup (Will be updated)
![RTI New Setup (Will be updated)](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/RTI-now.jpg)

RTI Old Setup
![RTI Old Setup](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/RTI-old.jpg)

### AUX Integration
While the Volvo S60 from 2008 typically comes equipped with an AUX input, my model had its radio upgraded to the HU-850, an older version lacking this feature. Following Karl Hagström's guide (https://gizmosnack.blogspot.com/2015/11/aux-in-volvo-hu-xxxx-radio.html), I created a CD changer emulator using an Arduino Nano clone.

![Radio Pinout](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/Radio%20Pins.png)

### RTI Extender Cable Scheme
![RTI Control Module](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/RTI%20Cable%20Extender%20Scheme.jpg)

### RTI Control Module Scheme
![RTI Control Module](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/RTI%20Control%20Module%20Scheme.jpg)

## Programming Details (SOON)

![Image Description](https://github.com/samelyuk/volvo-rti-retrofit/blob/main/doc/S60.jpg)
