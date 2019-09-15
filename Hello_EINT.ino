#include "Arduino.h"

int  ledPin = 13; 		// assign LED pin to number 13
int  digitalPin = 2;  	// assign digital pin to number 2
bool bLedState = LOW;	// initial LED state to LOW level
bool bBottonUP = true;	// initial bottonUP flag
bool bHasINT0 = false;	// initial INT0 flag

void setup()
{
    pinMode(ledPin, OUTPUT); 		 	// initial LED pin to OUTPUT mode
    pinMode(digitalPin, INPUT);	 		// initial digital pin to INPUT mode
    digitalWrite(ledPin, bLedState);	// setup LED state according to ledState

    Serial.begin(9600);
    	while (!Serial){
    		; // wait for serial port to connect
    	}
    Serial.println("Hello INT0");

    // config INT0 falling edge trigger (10) to generate interrupt by set EICRA
    EICRA |= _BV(ISC01);
    EICRA &= ~_BV(ISC00);
    // Enable INT0 interrupt by set EIMSK
    EIMSK |= _BV(INT0);
}

void loop()
{
	// read digital pin2 state and check bBottonUP
//  if ( digitalRead(digitalPin) == LOW && bBottonUP == true )
    if ( bHasINT0 && bBottonUP == true )
    {
    	bLedState = ! bLedState; 	 		// reverse LED state
    	digitalWrite(ledPin, bLedState);	// setup LED state according to ledState
    	bBottonUP = false;
    	delay(50);
   	}
// 	else if (digitalRead(digitalPin)== HIGH && bBottonUP == false)
   	else if (bHasINT0 && bBottonUP == false)
   	{
// 		Serial.println("re-enable INT0");
   		bBottonUP = true;
  		bHasINT0 = false;
  	    // Re-enable INT0 interrupt by set EIMSK
  	    EIMSK |= _BV(INT0);
    }
}

ISR(INT0_vect)
{
	EIMSK &= ~_BV(INT0);	// disable INT0
	bHasINT0 = true;		// Set flag
}
