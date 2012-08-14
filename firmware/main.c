#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv.h"

#define F_CPU 12000000L
#include <util/delay.h>
// this gets called when custom control message is received
#define USB_LED_OFF 0
#define USB_LED_ON  1
#define USB_DATA_OUT 2

USB_PUBLIC uchar usbFunctionSetup(uchar data[8]) {
    static uchar replyBuf[2];
    unsigned  int adc_value;
    usbRequest_t *rq = (void *)data; // cast data to correct type

    switch(rq->bRequest) { // custom command is in the bRequest field
    case USB_LED_ON:
        PORTB |= 1; // turn LED on
        return 0;
    case USB_LED_OFF: 
        PORTB &= ~1; // turn LED off
        return 0;
    case USB_DATA_OUT: // send data to PC
	    ADCSRA  |= (1<<ADSC);
		while (ADCSRA &  (1<<ADSC));
		adc_value = ADCW;
		replyBuf[0] = (char)(adc_value & 255);
		replyBuf[1] = (char)(adc_value >> 8);
        usbMsgPtr = replyBuf;
        return sizeof(replyBuf);    
    }

    return 0; // should not get here
}

int main() {
	
    uchar i;
    DDRB = 3; // PB0 as output
	ADCSRA  = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	ADMUX=0x05;
    wdt_enable(WDTO_1S); // enable 1s watchdog timer
    
    usbInit();
    PORTB ^= 2;
    usbDeviceDisconnect(); // enforce re-enumeration
    for(i = 0; i<250; i++) { // wait 500 ms
        wdt_reset(); // keep the watchdog happy
        _delay_ms(2);
    }
    usbDeviceConnect();
    PORTB ^= 2;
    sei(); // Enable interrupts after re-enumeration

    while(1) {
        wdt_reset(); // keep the watchdog happy
        usbPoll();
    }

    return 0;
}
/*void delay_ms( int ms ) 
{ 
    int p;
   for (p = 0; p < ms; p++) 
   { 
      _delay_ms(1); 
   } 
}
int main(void)
{
    DDRD = 255;           // make the LED pin an output 

    for(;;){
        int i;
        int k = 0;
        PORTD = k;
        for(i = 1; i < 9; i++){
            delay_ms(i * 1000);  // max is 262.14 ms / F_CPU in MHz 
            k = k + (1 << (i-1));
            PORTD = k;   // toggle the LED 
        }
    }
    return 0;              // never reached 
}
*/
