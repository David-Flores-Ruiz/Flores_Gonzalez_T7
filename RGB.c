/*
 * RGB.c
 *
 *  Created on: 10 sep 2019
 *      Author: Ricardo Pacas
 */

#include "MK64F12.h"
#include "RGB.h"
#include <GPIO.h>
#include "bits.h"

#define ASCENDENTE_SW2  (0x02u) // Orden1 de FSM
#define DESCENDENTE_SW3 (0x01u)	// Orden2 de FSM

typedef enum {
	GREEN,		// 0
	BLUE,		// 1
	PURPLE,		// 2
	RED,		// 3
	YELLOW,		// 4
}State_name_t;

uint8_t(*fptr_LED_ON_OFF[2])(color_ON color)={encender_LED,apagar_LED} ;

void fx (void){

	static int flag= 0;

	if(flag){
		fptr_LED_ON_OFF[1](RED_OFF);// apagamos
	}
	else{
		fptr_LED_ON_OFF[0](RED_ON);// enciende
	}

	flag = !flag;

}


uint8_t encender_LED(color_ON color_RGB) {
	switch (color_RGB) {
	case RED_ON:
		GPIO_clear_pin(GPIO_B, bit_22);	// ON - 0 RED
		break;
	case GREEN_ON:
		GPIO_clear_pin(GPIO_E, bit_26);	// ON - 0 GREEN
		break;
	case BLUE_ON:
		GPIO_clear_pin(GPIO_B, bit_21);	// ON - 0 BLUE
		break;
	case PURPLE_ON:
		GPIO_clear_pin(GPIO_B, bit_21);	// ON - 0 	BLUE +
		GPIO_clear_pin(GPIO_B, bit_22);	// ON - 0 	RED		= PURPLE
		break;
	case YELLOW_ON:
		GPIO_clear_pin(GPIO_B, bit_22);	// ON - 0 	RED +
		GPIO_clear_pin(GPIO_E, bit_26);	// ON - 0 	GREEN	= YELLOW
		break;
	default: /**If doesn't exist the option*/
		return (FALSE);
	}
	return (TRUE);
}

uint8_t apagar_LED(color_OFF color_RGB) {
	switch (color_RGB) {
	case RED_OFF:
		GPIO_set_pin( GPIO_B, bit_22);	// OFF - 1 RED
		break;
	case GREEN_OFF:
		GPIO_set_pin( GPIO_E, bit_26);	// OFF - 1 GREEN
		break;
	case BLUE_OFF:
		GPIO_set_pin( GPIO_B, bit_21);	// OFF - 1 BLUE
		break;
	case RGB_OFF:
		GPIO_set_pin( GPIO_B, bit_22);	// OFF - 1 RED	  +
		GPIO_set_pin( GPIO_E, bit_26);	// OFF - 1 GREEN  +
		GPIO_set_pin( GPIO_B, bit_21);	// OFF - 1 BLUE		=	RGB is OFF
		break;
	default: /**If doesn't exist the option*/
		return (FALSE);
	}
	return (TRUE);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
void delay(uint16_t delay) {
	volatile uint16_t j, i;

	for (j = delay; j > 0; j--) {
		for (i = 0; i < 1000; ++i) {
			__asm("nop");
		}

	}
}
