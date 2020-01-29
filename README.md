  

## Introduction

Facetrack is a project based on Raspberry Pi 3 Model B+ and  C++ language development. It is used for remote surveillance.  The project is under maintenance. Currently available version is v1.0. 

## Features

-  Detect faces and draw  face marks ;

-  The camera follows the tracked face;

- Remote surveillance through computer or mobile phone;

- Remote control the camera so that it can move upwards, downwards, left and right.

## Setup

```
$ git clone https://github.com/Yanxiall/facetrack.git
$ cd facetrack
$ mkdir build
$ cd build 
$ cmake ..
$ make
```

## Usage

1. Install the camera on the raspberry by connecting it to the Camera Serial Interface;

2. Connect the two servos to the GPIO interface of the raspberry pi ( in this project：GPIO.1 and GPIO.24 for PWM output )；

3. Open the command and input

   ```
   $ sudo ./facetrack
   ```

4. Open the file ws.html and input the IP address of Raspberry Pi, press the button "connect"; 

5. Press the buttons "up", "down", "left", "right" to control the movement of the servo;

6. Face the camera to test the function of face tracking.

## To Do

- Improve the performance of face tracking and remote control. 

-  Identify persons, which were already registered in the system and showed in the camera view. 

- Track a person’s face and make a photo if the person is not registered in system. 

- Send the photo to host’s phone once an unregistered person is found.

## What you need

#### Hardware

- Raspberry Pi 3 Model B+;

- A camera with two servos (one servo controls the camera moving up and down, another controls the camera moving left and right )  ;

#### Software

If you want to operate the Raspberry pi in a computer or mobile phone, then you need SSH client (e.g. Putty, Termius ) .

## Dependencies 

- Opencv 3.4.1
- Threads 
- Wiring Pi

## References

- https://github.com/kurnianggoro/GSOC2017

  







