# Agitalize - Magnetic stirrer

## Charcteristics
- Developed for arduino mega using arduino ide
- Uses LCD and Matrix keypad
- Has 4 indicative leds for progress of the process
- This project is one of the first 'big' projects I made
- Has a lot of fixes to do, such as clean use of LCD and keypad
- It is a simple project but it is useful, you can build one to use on chemestry laboratories
- Unfortunately the wiring is not available, but the specifications are on the code, it is a simple circuit

## Hardware
- The uC is the atmega2560 on arduino mega board
- The keyboard is a simple matrix keypad
- The LCD is a common LCD 16x2
- The DC motor is a simple motor with 2 wires
- The motor is connected to a external power supply through power mosfets

## Indicative LEDs
- The first led indicates that the system started
- The second led indicates that the user is setting values to the timers
- The third led indicates that the processes started
- The fourth led indicates that the processes finished (not forced to stop)
- If the system is stopped by pressing 'd', the 3 first leds blinks

## Overview
It is a simple project made to integrate knowledge of electronics and chemestry
It is open source and simple to reproduce
A magnetic stirrer is a tool used on chemestry labs that stirres some substances and this process can take a long time
with this project you can set the time you want for stirrings
Some magnectic stirrers has a heating too, if you want you can improve this project to do this
Another improvement you can do is to detect when the magnectic needle leaves its axis with magnetic sensors (hall efect)