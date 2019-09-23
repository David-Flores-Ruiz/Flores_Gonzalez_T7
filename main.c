/*
 * @file:			T01_RGB_SwInputs.c
 * @comany:			ITESO
 * @Engineer Team:	D.F.R. / R.G.P.
 */

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include <bits.h>
#include <GPIO.h>
#include "RGB.h"
#include <FSM.h>

int main(void) {

	uint32_t arriba_sw2 = 0;	// Secuencia: Verde-Amarillo-Rojo-Morado-Azul...Verde
	uint32_t abajo_sw3  = 0;	// Secuencia: Verde-Azul-Morado-Rojo-Amarillo...Verde
	uint32_t contador   = 0;	// Ascendente
	uint32_t contador_2 = 0;	// Descendente

	GPIO_clock_gating( GPIO_A);	// sw3
	GPIO_clock_gating( GPIO_B);	// led azul y rojo
	GPIO_clock_gating( GPIO_C);	// sw2
	GPIO_clock_gating( GPIO_E);	// led verde

	gpio_pin_control_register_t led_config = GPIO_MUX1;						// 100
	gpio_pin_control_register_t sw_config  = GPIO_MUX1 |GPIO_PE | GPIO_PS;	// 103

	GPIO_pin_control_register( GPIO_A, bit_4,  &sw_config );  // sw3	  - pin  4
	GPIO_pin_control_register( GPIO_B, bit_21, &led_config);  // ledBlue  - pin 21
	GPIO_pin_control_register( GPIO_B, bit_22, &led_config);  // ledRed   - pin 22
	GPIO_pin_control_register( GPIO_C, bit_6,  &sw_config );  // sw2	  - pin  6
	GPIO_pin_control_register( GPIO_E, bit_26, &led_config);  // ledGreen - pin 26

	GPIO_set_pin( GPIO_B, bit_22);	// OFF - 1 RED
	GPIO_set_pin( GPIO_E, bit_26);	// OFF - 1 GREEN
	GPIO_set_pin( GPIO_B, bit_21);	// OFF - 1 BLUE

	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, bit_22); // OUTPUT - 1 RED
	GPIO_data_direction_pin(GPIO_E, GPIO_OUTPUT, bit_26); // OUTPUT - 1 GREEN
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, bit_21); // OUTPUT - 1 BLUE

	while (1) {





	} // end while (1)
} // end main

