#include "contiki.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "button-sensor.h"
#include "board-peripherals.h"
#include "cc26xx-uart.h"
#include "sys/mt.h"
#include "ti-lib.h"

#include <stdio.h>
#include <stdint.h>

#define CC26XX_DEMO_LOOP_INTERVAL       (CLOCK_SECOND / 2)

#define DEV_LED_WHITE					BOARD_IOID_DP0
#define DEV_LED_GREEN 					BOARD_IOID_DP1
#define DEV_LED_BLUE 					BOARD_IOID_DP2
#define DEV_LED_RED 					BOARD_IOID_DP3


static struct etimer et;
static int counterA = 0 ;

PROCESS(button_process, "button process");
PROCESS(ledpack_process, "led audio devpack process");

PROCESS(cc26xx_contiki_demo_process, "cc26xx contiki demo process");
AUTOSTART_PROCESSES(&cc26xx_contiki_demo_process, &button_process, &ledpack_process);

int uart_rx_callback(unsigned char c)
{
	cc26xx_uart_write_byte(c);
	return 1;
}

void thread_entry(void *data)
{
	int counterB = 0;

	while(1) {
		printf("Looping in thread_entry %d\n", counterB);
		counterB++;
		//mt_yield();
	}
}

PROCESS_THREAD(cc26xx_contiki_demo_process, ev, data)
{
	//static struct mt_thread thread;

	PROCESS_BEGIN();
	
	//mt_start(&thread, thread_entry, NULL);

	printf("Hello Contiki!!!\nCC26XX contiki demo\n");

	cc26xx_uart_set_input(uart_rx_callback);

	etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);

	while(1) {

		if(counterA == 0) {
			leds_off(LEDS_RED);
			leds_on(LEDS_GREEN);

		}

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		if(ev == PROCESS_EVENT_TIMER) {
			if(data == &et) {
				counterA++;

				leds_toggle(LEDS_ALL);
				

				//if(counterA % 2) {
				//	printf("Starting thread\n");
				//	mt_exec(&thread);
				//}

				printf("500msecs passed, time elapsed: %ds\n", (int)(counterA*0.5));
				etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);
			}
		} 
	}

	PROCESS_END();

}


PROCESS_THREAD(button_process, ev, data)
{

	PROCESS_BEGIN();

	printf("Hello from button process\n");

	while(1) {

		PROCESS_YIELD();

		if(ev == sensors_event && data == &button_left_sensor ) {
			printf("Left button pushed\n");
			//watchdog_reboot();
		} else if(ev == sensors_event && data ==  &button_right_sensor) {
			printf("Right button pushed, time: %ds\n", (int)(counterA*0.5));
			if(buzzer_state()) {
				buzzer_stop();
			} else {
				//buzzer_start(1000);
			}
		}

	}

	PROCESS_END();

}

PROCESS_THREAD(ledpack_process, ev, data)
{

	PROCESS_BEGIN();

	printf("Hello from led process\n");


	while(1) {

		ti_lib_rom_ioc_pin_type_gpio_output(DEV_LED_WHITE);
		ti_lib_gpio_pin_write(DEV_LED_WHITE, 1);

		ti_lib_rom_ioc_pin_type_gpio_output(DEV_LED_RED);
		ti_lib_gpio_pin_write(DEV_LED_RED, 0);

		ti_lib_rom_ioc_pin_type_gpio_output(DEV_LED_GREEN);
		ti_lib_gpio_pin_write(DEV_LED_GREEN, 1);

		PROCESS_YIELD();

	}

	PROCESS_END();

}