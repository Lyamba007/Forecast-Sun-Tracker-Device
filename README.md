# Staircase-Lighting-System

A microcontroller device for controlling stair lighting based on the principle of full event programming with the implementation of a finite control machine has been developed.
The control unit is implemented on the basis of the microcontroller family STM32 (STM32F103C8T6). 
The software implementation of the control algorithm is carried out in the C programming language in the Keil development environment and with the help of CubeMx CAD.
Based on the protocol of data transmission according to official documentation and the use of modern solutions for interaction with RGB-LEDs, the connection between the MCU and the LED strip was implemented using DMA technology to minimize the hardware costs of the microcontroller.
The prototype of the stair lighting system in a private residential building is implemented on the basis of a simplified circuit implementation using LED strip based on WS2812B LEDs using a debug board STM32.
____________________________________________________________________________________________________________________________________________
# Area of improvement

It is possible to base the prototype solution on the cheapest microcontroller in STM32 series STM32F030C8T6.
The mathematical part of code such as day of the year, zenith angle calculation and LED lighting in prototype realization should be implemented out of interrupt handler. 
The hardware RTC-module should be added to follow instructions at the right and configured time. 
____________________________________________________________________________________________________________________________________________
# Repository content

The repository consist of:
Control Device C project based on STM32F103C8T6 MCU (Predict sun tracker device)
Project specification in Ukrainian
Presentation notes in English
Demo Video. The prototype work demonstration
Comments to Demo Video in English
____________________________________________________________________________________________________________________________________________
