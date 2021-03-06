/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 * 
 * DESCRIPTION
 * Example sketch showing how to control physical relays. 
 * This example will remember relay state after power failure.
 * http://www.mysensors.org/build/relay
 */ 

// Enable debug prints to serial monitor
#define MY_DEBUG 
#define MY_NODE_ID 2
// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// When W5100 is connected we have to move CE/CSN pins for NRF radio
#ifndef MY_RF24_CE_PIN 
  #define MY_RF24_CE_PIN 7
#endif
#ifndef MY_RF24_CS_PIN 
  #define MY_RF24_CS_PIN 8
#endif

// Enable repeater functionality for this node
//#define MY_REPEATER_FEATURE

//#include <SPI.h>
#include <MySensors.h>
//#include <Bounce2.h>

#define RELAY_1  5  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_RELAYS 1 // Total number of attached relays
#define RELAY_ON 0  // GPIO value to write to turn on attached relay
#define RELAY_OFF 1 // GPIO value to write to turn off attached relay
//#define BUTTON_PIN  4  // Arduino Digital I/O pin for button/reed switch

//Bounce debouncer = Bounce(); 
//int oldValue=-1;


MyMessage Relay1(1,V_LIGHT);
//MyMessage Relay2(2,V_LIGHT);

unsigned long timeA;

void before() { 
  Serial.println("--> Leyendo valores desde EEPROM :");
  for (int i=0;i<=255;i++) {
      Serial.println("---> Sensor " + String(i) + " Valor:" + String(loadState(i)));
  }
  Serial.println("--> Lectura completada"); 
  
  for (int sensor=1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    // Then set relay pins in output mode
    pinMode(pin, OUTPUT);
    digitalWrite(pin, RELAY_OFF);
    // Set relay to last known state (using eeprom storage) 
    //digitalWrite(pin, loadState(sensor)?RELAY_ON:RELAY_OFF);
  }
}

void setup() {
  // Setup the button
  //pinMode(BUTTON_PIN,INPUT);
  // Activate internal pull-up
  //digitalWrite(BUTTON_PIN,HIGH);

  // After setting up the button, setup debouncer
  //debouncer.attach(BUTTON_PIN);
  //debouncer.interval(5);
}

void presentation()  
{   
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Garage Door Actuator", "1.0");

  for (int sensor=1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_LIGHT);
  }
}


void loop() 
{
/*    debouncer.update();
    // Get the update value
    int value = debouncer.read();

    if (value != oldValue) {
      // Change relay state
    //  if (value==LOW) {
        digitalWrite(RELAY_1, !digitalRead(RELAY_1));
        // Store state in eeprom
        saveState(1, digitalRead(RELAY_1));
        // Write some debug info
        Serial.print("Incoming change for sensor from button: 1");
        Serial.print(", New status: ");
        Serial.println(digitalRead(RELAY_1));
        // Send in the new value
        send(Relay1.set(digitalRead(RELAY_1)));
    //  }
     oldValue = value;
    }
*/
   // publish & update reading every 30 seconds
    if (millis() > (timeA + 30000)) {
      timeA = millis();
      send(Relay1.set(digitalRead(RELAY_1)));
      //send(Relay2.set(digitalRead(RELAY_1+1)));
    }
}

void receive(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_LIGHT) {
     // Change relay state
     digitalWrite(message.sensor-1+RELAY_1, RELAY_ON);
     wait(200);
     digitalWrite(message.sensor-1+RELAY_1, RELAY_OFF);
     // Store state in eeprom
     //saveState(message.sensor, message.getBool());
     // Write some debug info
     Serial.print("Incoming change for sensor:");
     Serial.print(message.sensor);
     Serial.print(", New status: ");
     Serial.println(message.getBool());
     send(Relay1.set(digitalRead(RELAY_1)));
   } 
}

