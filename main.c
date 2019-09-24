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

#define ASCENDENTE_SW2  (0x02u)
#define DESCENDENTE_SW3 (0x01u)

typedef enum {
	GREEN,		// 0
	BLUE,		// 1
	PURPLE,		// 2
	RED,		// 3
	YELLOW,		// 4
}State_name_t;

int main(void) {

	State_name_t current_state = GREEN;	// Estado Inicial de la FSM
	uint32_t arriba_sw2  = 0;	// Secuencia: Verde-Amarillo-Rojo-Morado-Azul...Verde
	uint32_t abajo_sw3   = 0;	// Secuencia: Verde-Azul-Morado-Rojo-Amarillo...Verde
	uint32_t total_input = 0;

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
		arriba_sw2  = GPIO_read_pin( GPIO_C, bit_6);	// lee el sw2
		abajo_sw3   = GPIO_read_pin( GPIO_A, bit_4);	// lee el sw3
		arriba_sw2  = arriba_sw2 >> 6;		// mandar el bit al LSB
		abajo_sw3   = abajo_sw3  >> 3;		// mandar el bit al penúltimo
		total_input = arriba_sw2 | abajo_sw3;

		switch (current_state)
		{
			case GREEN:
				apagar_LED(RGB_OFF);
				encender_LED(GREEN_ON);			// ENCIENDE LED GREEN EN k-64

				switch (total_input)
				{
					case ASCENDENTE_SW2:
						current_state = YELLOW;		// next en secuencia ascendente
						break;
					case DESCENDENTE_SW3:
						current_state = BLUE;		// next en secuencia descendente
						break;
					default:
						current_state = GREEN;		// secuencia INVÁLIDA no cambia el next
					break;
				}
				break;// end case GREEN

			case BLUE:
				apagar_LED(RGB_OFF);
				encender_LED(BLUE_ON);			// ENCIENDE LED BLUE EN k-64

				switch (total_input)
				{
					case ASCENDENTE_SW2:
						current_state = GREEN;		// next en secuencia ascendente
						break;
					case DESCENDENTE_SW3:
						current_state = PURPLE;		// next en secuencia descendente
						break;
					default:
						current_state = BLUE;		// secuencia INVÁLIDA no cambia el next
					break;
				}
				break;// end case BLUE

				case PURPLE:
				apagar_LED(RGB_OFF);
				encender_LED(PURPLE_ON);			// ENCIENDE LED PURPLE EN k-64

				switch (total_input)
				{
					case ASCENDENTE_SW2:
						current_state = BLUE;		// next en secuencia ascendente
						break;
					case DESCENDENTE_SW3:
						current_state = RED;		// next en secuencia descendente
						break;
					default:
						current_state = PURPLE;		// secuencia INVÁLIDA no cambia el next
					break;
				}
				break;// end case PURPLE

				case RED:
				apagar_LED(RGB_OFF);
				encender_LED(RED_ON);			// ENCIENDE LED RED EN k-64

				switch (total_input)
				{
					case ASCENDENTE_SW2:
						current_state = PURPLE;		// next en secuencia ascendente
						break;
					case DESCENDENTE_SW3:
						current_state = YELLOW;		// next en secuencia descendente
						break;
					default:
						current_state = RED;		// secuencia INVÁLIDA no cambia el next
					break;
				}
				break;// end case RED

				case YELLOW:
				apagar_LED(RGB_OFF);
				encender_LED(YELLOW_ON);			// ENCIENDE LED YELLOW EN k-64

				switch (total_input)
				{
					case ASCENDENTE_SW2:
						current_state = RED;		// next en secuencia ascendente
						break;
					case DESCENDENTE_SW3:
						current_state = GREEN;		// next en secuencia descendente
						break;
					default:
						current_state = YELLOW;		// secuencia INVÁLIDA no cambia el next
					break;
				}
				break;// end case YELLOW

		}//end switch (current state)


	} // end while (1)
} // end main

