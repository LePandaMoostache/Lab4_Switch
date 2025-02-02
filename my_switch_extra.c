#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define 	RED_MASK 		0x02	// PF1, 2^1 = 2
#define   BLUE_MASK   0x04	// PF2, 2^2 = 4 
#define   GREEN_MASK  0x08	// PF3, 2^3 = 8

#define		SW2					0x01 // PF0, 2^0 = 1
#define 	SW1					0x10 // PF4, 2^4 = 16

//*****************************************************************************
//
//!
//! A very simple example that interfaces with the red LED (PF2) and SW2 (PF0) 
//! using direct register access. When SW2 is pressed, the LED is turned on. When 
//! SW2 is released, the LED is turned off. 
//
//*****************************************************************************

void
PortFunctionInit(void)
{
		volatile uint32_t ui32Loop;   
	
		// Enable the clock of the GPIO port that is used for the on-board LED and switch.
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ui32Loop = SYSCTL_RCGC2_R;

		// Unlock GPIO Port F
		GPIO_PORTF_LOCK_R = 0x4C4F434B;    // Convert to ASCII -> "LOCK"
	
}

void
SW2Function(void)
{

		GPIO_PORTF_CR_R |= SW2;           // allow changes to PF0

    // Set the direction of PF2 (red LED) as output
    GPIO_PORTF_DIR_R |= RED_MASK;
	
		// Set the direction of PF0 (SW2) as input by clearing the bit
    GPIO_PORTF_DIR_R &= ~SW2;					// NOT pointing at SW2 at that time
	
    // Enable both PF2 and PF0 for digital function.
    GPIO_PORTF_DEN_R |= 0x03; 				// Addition? (3)
	
		//Enable pull-up on PF0
		GPIO_PORTF_PUR_R |= SW2;

}

void
SW1Function(void)
{

		GPIO_PORTF_CR_R |= SW1;          // allow changes to PF4

    // Set the direction of PF3 (green LED) as output
    GPIO_PORTF_DIR_R |= GREEN_MASK;
	
		// Set the direction of PF4 (SW1) as input by clearing the bit
    GPIO_PORTF_DIR_R &= ~0x10; // NOT pointing at SW1 that time
	
    // Enable both PF3 and PF4 for digital function.
    GPIO_PORTF_DEN_R |= 0x18;				// Addition? (24)
	
		//Enable pull-up on PF4
		GPIO_PORTF_PUR_R |= SW1; 

}

int main(void)
{
	
		//initialize the GPIO ports
		PortFunctionInit();
		SW2Function();
		SW1Function();
	
    //
    // Loop forever.
    //

    while(1)
    {

				
				if((GPIO_PORTF_DATA_R&SW2 && GPIO_PORTF_DATA_R&SW1) !=0x00) // Turn red and green ON
				{
				GPIO_PORTF_DATA_R |= GREEN_MASK;
				GPIO_PORTF_DATA_R |= RED_MASK;
						
				}
				else if((GPIO_PORTF_DATA_R&SW1) !=0x00) // SW1 NOT Pressed?
				{
						// Turn OFF the RED LED.
						GPIO_PORTF_DATA_R &= ~RED_MASK;
				}
				else if((GPIO_PORTF_DATA_R&SW2) !=0x00) // SW2 NOT Pressed?
				{
						// Turn OFF the GREEN LED.
						GPIO_PORTF_DATA_R &= ~GREEN_MASK;
				}
				else
				{
						// Turn OFF the LED.
						GPIO_PORTF_DATA_R |= 0x00;
				}	
				
				
				/*
				if((GPIO_PORTF_DATA_R&SW2)!=0x00) // Is SW2 currently NOT pressed?
				{
						// Turn ON the LED.
						GPIO_PORTF_DATA_R |= RED_MASK;
						
				}
				else
				{
						// Turn OFF the LED.
						GPIO_PORTF_DATA_R &= ~RED_MASK;
				}
				*/
				
    }
}
