#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "shift.h"
#include "stepper.h"

// ----------- Input Defines ----------- //
#define topPORT PORTD
#define topDDR  DDRD
#define topPIN  PIND
#define top     PD0

#define tipPORT PORTD
#define tipDDR  DDRD
#define tipPIN  PIND
#define tip     PD2 // because it will use INT0

#define engagePORT PORTD
#define engageDDR  DDRD
#define engagePIN  PIND
#define engage     PD3 // because it will use INT1 (Probably. I haven't thought much about it yet.)

// ----------- Touch LED Defines ----------- //
#define touchLEDPORT PORTD
#define touchLEDDDR  DDRD
#define touchLED     PD7

// Global to keep track of which mode the user is on.
uint8_t mode = 0;

void choose_mode() {
  
  // Let user know they are changing modes by flashing 4 times.
  shift_flash(4);
  
  while (bit_is_clear(tipPIN, tip)) {
    // Entered this function by holding the tip button, so wait until it is
    // released before beginning to choose.
  }
  
  // Counter for how long button is presssed
  uint16_t millisHeld;
  
  // Keep cycling the mode until the user holds the tip button for 1 second
  while (1) {      
    // Display mode on LEDs
    shift_write_single(mode);
    
    millisHeld = 0;
    while (bit_is_clear(tipPIN, tip)) {
      millisHeld++;
      if (millisHeld > 1000) {break;}
      _delay_ms(1);
    }
    
    if (millisHeld > 1000) {
      shift_flash(4);
      break;
    } else if (millisHeld > 20) {
      // If button was pressed more than 20ms it was a real press, not a bounce.
      
      // Increment the mode, wrap to zero if it overflows
      if (mode++ >= 7) {
        mode = 0;
      }

    }
  }
}


void reset_fencebot() {
  while(bit_is_set(topPIN, top)) {
    step_up();
    _delay_ms(5); // so we don't overstep
  }
  
/*  If waiting for the sensor alone is insufficient, we can then go until a specific step.*/
/*  while(currentStep != 0) {*/
/*    step_up();*/
/*    _delay_ms(5); // so we don't overstep*/
/*  }*/
}


// ----------- Hardware interrupt for the tip button ----------- //
ISR(INT0_vect) {
  // Turn the touchLED on and keep it on until fencebot has reset.
  touchLEDPORT |= (1 << touchLED);
  
  // Test how long the button is held. Fencebot will remain stationary this whole time.
  uint8_t changingMode = 1;
  for (uint8_t i = 0; i < 50; i++) {
    // Wait first to debounce switch
    _delay_ms(20);
    
    // If they release the button before the threshold, they are not changing mode.
    if (bit_is_set(tipPIN, tip)) {
      changingMode = 0;
      break;
    }
  }
  
  if (changingMode) {
    choose_mode();
  }
    
  // Reset fencebot to top, and reset any global or perry related variables
  reset_fencebot();  
  
  // Turn the touchLED back off.
  touchLEDPORT &= ~(1 << touchLED);
  
}




 
void main() {

  // ----------- Initialization -----------  //
  
  // External initilization
  motor_init();
  shift_init();
  
  // Set up interrupt system
  EIMSK |= (1 << INT0);  // Enable fancy hardware interrupt INT0
  EICRA |= (1 << ISC00); // Trigger interrupt on rising or falling edge
  sei();                 // Set global interrupt flag on
  
  // Setup buttons and LEDs
  touchLEDDDR |= (1 << touchLED); // Set LED for output
  tipPORT |= (1 << tip);          // Enable internal pullup on tip button.
  topPORT |= (1 << top);          // Enable internal pullup on top sernsor.
  
  //TODO initialize some timer here and connect a timer interrupt to it.
  // (If you decide to take the rotation stuff out of the main loop.)

 // Main program loop
  while(1) {
    //TODO Move this particular perry into a separate function
    rotate(360, 2500);
    //shift_flash(2);
  }
  
  return;
}
