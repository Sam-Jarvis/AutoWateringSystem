# AutoWateringSystem
![Image of msn-lsn](https://github.com/Sam-Jarvis/AutoWateringSystem/blob/master/ImagesForDocumentation/AutoWaterer.png?raw=true)<br>
__Video example:__ https://youtu.be/DMxAB1ufoSI<br>

### This repository contains 3D designs, code and a technical report of an automatic plant watering system I built.<br>

The system is designed and built to be run on an arduino, thus it is programmed in C using the Arduino IDE. The purpose of the system is to water herbs or other plants without human interaction. This is based on a timer that can be set by the user. The system can water a set number of plants or it can adapt to the number of plants on the fly given that an ultrasonic sensor is connected.<br>

The parts necessary to build the system are:
* Servo (small)
* Arduino
* Jumper wires
* Ultrasonic sensor _opt_
* 9V battery
* Relay
* Aquarium pump

To summarize the system, the relay control an aquarium pump that is placed in a reservoir of water. The relay is controlled by the Arduino which is powered by the 9V battery. The time interval between watering is set using the potentiometer.<br>  

The parts are assembled inside the 3D printed base and connected together like this (also in the technical report):
### Circuit Diagram
![Image of msn-lsn](https://github.com/Sam-Jarvis/AutoWateringSystem/blob/master/ImagesForDocumentation/CircuitDiagram.png?raw=true)<br>

### Example Circuit
![Image of msn-lsn](https://github.com/Sam-Jarvis/AutoWateringSystem/blob/master/ImagesForDocumentation/CircuitExample.png?raw=true)<br>
