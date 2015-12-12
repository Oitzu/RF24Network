/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network 
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,8);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already

byte data[24];
unsigned long counter, rxTimer;          //Counter and timer for keeping track transfer info
unsigned long startTime, stopTime;

struct payload_t {                  // Structure of our payload
  unsigned long ms;
  unsigned long counter;
};

void setup(void)
{
  
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_tx/");
 
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  
  
  
  for(int i=0; i<24; i++){
     data[i] = random(255);               //Load the buffer with random data
  }
  radio.printDetails();
}

void loop() {
  delay(2000);
  network.update();                          // Check the network regularly

   Serial.println(F("Initiating Basic Data Transfer"));
   
       unsigned long cycles = 10000; //Change this to a higher or lower number. 
    RF24NetworkHeader header(/*to node*/ other_node);
    startTime = millis();
    unsigned long pauseTime = millis();
    
    for(int i=0; i<cycles; i++){        //Loop through a number of cycles
      data[0] = i;                      //Change the first byte of the payload for identification
      if(!network.write(header,&data,sizeof(data))){   //Write to the FIFO buffers        
        counter++;                      //Keep count of failed payloads
      }
      
    }
    
   stopTime = millis();   
   
   float numBytes = cycles*32;
   float rate = numBytes / (stopTime - startTime);
    
   Serial.print("Transfer complete at "); Serial.print(rate); Serial.println(" KB/s");
   Serial.print(counter); Serial.print(" of "); Serial.print(cycles); Serial.println(" Packets Failed to Send");
   counter = 0;  
}

