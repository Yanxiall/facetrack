## Introduction

This project provides an intelligent remote surveillance system by using raspberry pi 3b+ as domain controller with servo driven 2 axis camera. With this system you can monitor your home at any place with a web based client that applicable for both phone and pc. The system also provides an intelligent operation mode that tracks detected person automatically by using AI algorithm and notify the system admin.

## Features

- Video real time streaming.
- Remote control of the camera.
- Detected person automatic tracking and notification.
- Detected person photos saving and Photo display on web client.

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

3. Run program: Open the command and input

   ```
   $ sudo ./src/facetrack
   ```

4. Open the web client and login; 

![Usage example](usage-example.gif)


## Unit test
- Google test is used for unit test, to run unit test, run command:

   ```
   $  ./test/facetrackTest
   ```

## What you need

#### Hardware

- Raspberry Pi 3 Model B+;

- A camera with 2 axis servos (one servo controls the camera moving up and down, another controls the camera moving left and right )  ;

#### Software

If you want to operate the Raspberry pi in a computer or mobile phone, then you need SSH client (e.g. Putty, Termius ) .

## Dependencies 

- Opencv 3.4.1
- Threads 
- Wiring Pi
- libmariadb-dev
- libgtest-dev

## References

- https://github.com/kurnianggoro/GSOC2017
- https://github.com/opencv/opencv/tree/master/data/haarcascades
