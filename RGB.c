/*
 * RGB.c
 *
 *  Created on: 10 sep 2019
 *      Author: Ricardo Pacas
 */

#include "MK64F12.h"
#include "bits.h"
#include "RGB.h"
#include <GPIO.h>
#include "PIT.h"

static void (*PIT_0_callback)(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay);
static void (*PIT_1_callback)(void) = 0;	// Inicializamos apuntadores a función
static void (*PIT_2_callback)(void) = 0;
static void (*PIT_3_callback)(void) = 0;

#define SYSTEM_CLOCK (21000000U)
#define DELAY (1)				// Cambio de estados por PIT cada 1 seg


void PIT_callback_init(PIT_timer_t pit_timer, void (*handler)(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)) {
	if (PIT_0 == pit_timer) {
		PIT_0_callback = handler;
	}
	if (PIT_1 == pit_timer) {
		PIT_1_callback = handler;
	}
	if (PIT_2 == pit_timer) {
		PIT_2_callback = handler;
	}
	if (PIT_3 == pit_timer) {
		PIT_3_callback = handler;
	}
}

void PIT0_IRQHandler(void)			// ESTE ES MI VECTOR DE INTERRUPCIÓN
{
	if (PIT_0_callback) {	// Si ya se inicializó el apuntador a función
		PIT_0_callback(PIT_0, SYSTEM_CLOCK, DELAY);	// con el Handler del CallBack... LLAMADA A FUNCIÓN: PIT_delay();
	}

	PIT_clear_interrupt(PIT_0);				// Apago interrupción por HW
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
