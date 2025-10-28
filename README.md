# Arduino-Radio-Modem-
A USB microcontroller device that will control hardware peripherals for a raspberry pi central OS (which will be written using the circle project) 

# Development:  
  10.28.2025: I need to pause the project due to budgeting and leraning constraints. I fully intend to keep developing the project.  
  
# Learning / Resources  
  https://randomnerdtutorials.com/projects-esp32/  
  
# Vision:  
  Utilize a circle project custom C++ kernel and a raspberry pi 3 B+ with a teensy 4.0 to make a laptop similar device with a  
  long range 900Mhz telementry UART modem and lots of interesting stuff - even HDMI display.  
  The device will be a playground for communicating with RC equipment that I design myself, so I'll be able to design my own packets  
  and integrate any sensors I want, make the laptop GUI however I want, etc. Intended to be a long standing project to practice OS  
  programming. 
  I ahve verified all critical aspects of the project such as power supply, circle project capabilities, and MCU capabilities. The  
  hard part is learning enough of the circle project to actually put this together, and having money of course. 
# Description 
  This project will be migrated to the Teensy when I get my paycheck =)  
  I intend to continue developing arduino code for the Teensy which can be found here.  
  The project essentially will be a Teensy that is pretending to be a USB device and feeding a raspberry pi the proper info from  
  GPS, buttons, and even accelerometer. The Teensy will also control an addressable LED for indicating things, and also it will  
  control a buzzer that makes tones for alerting the user to errors and button presses, radio transmissions, etc. 
# Pre-Requisites
  Designed for ESP32 Wroom-32 (Will be moved to Teensy ASAP) 
  Written in Arduino C++
# Wiring  
  Power LED  
  Buttons  
  TFT  
  Buzzer 
# Video Demo 
