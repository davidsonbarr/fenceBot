#include <avr/io.h>
//TODO remove delay dependency?
#include <util/delay.h>
#include "shift.h"

#define shiftPORT  PORTB
#define shiftDDR   DDRB

#define shiftCLOCK PB1
#define shiftLATCH PB2
#define shiftDATA  PB3




void shift_init() {
  //Make the Data, Clock, and Latch lines output
  shiftDDR|=((1<<shiftCLOCK)|(1<<shiftLATCH)|(1<<shiftDATA));
}

//Sends a pulse on Clock line
static void pulse() {
  shiftPORT|=(1<<shiftCLOCK);//HIGH
  shiftPORT&=(~(1<<shiftCLOCK));//LOW
}

//Sends a pulse on Latch line
static void latch() {
  shiftPORT|=(1<<shiftLATCH);//HIGH
  _delay_loop_1(1);

  shiftPORT&=(~(1<<shiftLATCH));//LOW
  _delay_loop_1(1);
}

void shift_write_byte(uint8_t data)
{
  //Send each 8 bits serially

  //Order is MSB first
  for(uint8_t i=0;i<8;i++)
  {
    // Output the MSB on the data line
    if(data & 0b10000000)
    {
      shiftPORT |= (1 << shiftDATA);
    }
    else
    {
      shiftPORT &= (~(1<<shiftDATA));
    }

    pulse();  // Pulse the Clock line
    data=data<<1;  // Now bring next bit to MSB position

  }

  // All 8 bits have been transferred to shift register, so latch them.
  latch();
}

void shift_write_single(uint8_t number) {
  // Used to light a single LED
  if (number <= 8) {
    shift_write_byte(1 << number);
  }
  else { // Try to help debug when invalid nubmer is written.
    shift_flash(2);
  }
  
}

void shift_flash(uint8_t times) {
  
  for (uint8_t i = 0; i < times; i++) {
   // Wait Briefly
    for (uint16_t i = 0; i < 0xffff; i++) {}  
    
    // Turn al LEDs on
    shift_write_byte(0xff);
    
    // Wait briefly
    for (uint16_t i = 0; i < 0xffff; i++) {}

    // Turn all LEDs off
    shift_write_byte(0x00);
  }
}

//TODO Maybe a shift_in(bit)
// But only if I need it.

