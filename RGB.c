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


const uint8_t ENCIENDE = 0;
const uint8_t APAGA = 1;


typedef enum {
	GREEN,		// 0
	BLUE,		// 1
	PURPLE,		// 2
	RED,		// 3
	YELLOW,		// 4
} State_name_t;

uint8_t(*fptr_LED_ON_OFF[2])(color_ON color)={encender_LED,apagar_LED};

void fx(void) {

	uint32_t arriba_sw2 = 0;// Secuencia: Verde-Amarillo-Rojo-Morado-Azul...Verde
	uint32_t abajo_sw3 = 0;
	static uint32_t contador = 0;// Secuencia: Verde-Azul-Morado-Rojo-Amarillo...Verde
	static uint8_t flag_asciende = 0;
	static uint8_t flag_desciende = 0;

	for (;;) {
		arriba_sw2 = GPIO_read_pin(GPIO_C, bit_6);	// lee el sw2
		abajo_sw3 = GPIO_read_pin(GPIO_A, bit_4);	// lee el sw3
		fptr_LED_ON_OFF[APAGA](RGB_OFF);	// apagamos


		if (contador == 5) {
			contador = 0;
		} else if (contador > 5) {
			contador = 4;
		}

		if (abajo_sw3 == FALSE) {
			if (flag_asciende == FALSE) {
				fptr_LED_ON_OFF[ENCIENDE](contador);	// enciende
				contador++;
				flag_desciende = 1;
			} else {
				contador = contador + 2;
				fptr_LED_ON_OFF[ENCIENDE](contador);	// quita bug
				flag_asciende = 0;			   // ya cambió de secuencia
			}
		}
		if (arriba_sw2 == FALSE) {
			if (flag_desciende == FALSE) {
				fptr_LED_ON_OFF[ENCIENDE](contador);			   // quita bug
				contador--;
				flag_asciende = 1;
			} else {
				contador = contador - 2;
				fptr_LED_ON_OFF[ENCIENDE](contador);			   // quita bug
				flag_desciende = 0;			   // ya cambió de secuencia
			}
		}

		break;
	}
}

uint8_t encender_LED(color_ON color_RGB) {
	switch (color_RGB) {
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
	case RED_ON:
		GPIO_clear_pin(GPIO_B, bit_22);	// ON - 0 RED
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
