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
#define CC26XX_LED_PACK_INTERVAL       (CLOCK_SECOND / 5)

#define DEV_LED_IOID_WHITE				BOARD_IOID_DP0
#define DEV_LED_IOID_GREEN 				BOARD_IOID_DP1
#define DEV_LED_IOID_BLUE 				BOARD_IOID_DP2
#define DEV_LED_IOID_RED 				BOARD_IOID_DP3

#define DEV_LED_WHITE					(1 << DEV_LED_IOID_WHITE)
#define DEV_LED_GREEN 					(1 << DEV_LED_IOID_GREEN)
#define DEV_LED_BLUE 					(1 << DEV_LED_IOID_BLUE)
#define DEV_LED_RED 					(1 << DEV_LED_IOID_RED)

static struct etimer et, et2;
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
		} else if(ev == sensors_event && data ==  &button_right_sensor) {
			printf("Right button pushed, time: %ds\n", (int)(counterA*0.5));
		}

	}

	PROCESS_END();

}

void led_pwm(int freq, uint32_t ioid_pin) {
	uint32_t load;

	/* Enable GPT0 clocks under active, sleep, deep sleep */
	ti_lib_prcm_peripheral_run_enable(PRCM_PERIPH_TIMER0);
	ti_lib_prcm_peripheral_sleep_enable(PRCM_PERIPH_TIMER0);
	ti_lib_prcm_peripheral_deep_sleep_enable(PRCM_PERIPH_TIMER0);
	ti_lib_prcm_load_set();
	while(!ti_lib_prcm_load_get());

	/* Drive the I/O ID with GPT0 / Timer A */
	ti_lib_ioc_port_configure_set(DEV_LED_IOID_WHITE, IOC_PORT_MCU_PORT_EVENT0,
	                            IOC_STD_OUTPUT);

	/* GPT0 / Timer A: PWM, Interrupt Enable */
	HWREG(GPT0_BASE + GPT_O_TAMR) = (TIMER_CFG_A_PWM & 0xFF) | GPT_TAMR_TAPWMIE;

	ti_lib_timer_disable(GPT0_BASE, TIMER_A);

    load = (GET_MCU_CLOCK / freq);

    ti_lib_timer_load_set(GPT0_BASE, TIMER_A, load);
    ti_lib_timer_match_set(GPT0_BASE, TIMER_A, load / 2);

    /* Start */
    ti_lib_timer_enable(GPT0_BASE, TIMER_A);

}

PROCESS_THREAD(ledpack_process, ev, data)
{

	PROCESS_BEGIN();

	printf("Hello from led process\n");

	led_pwm(10, DEV_LED_IOID_WHITE); 

	// ti_lib_rom_ioc_pin_type_gpio_output(DEV_LED_IOID_WHITE);
	// ti_lib_rom_ioc_pin_type_gpio_output(DEV_LED_IOID_RED);
	// ti_lib_rom_ioc_pin_type_gpio_output(DEV_LED_IOID_GREEN);
	// ti_lib_rom_ioc_pin_type_gpio_output(DEV_LED_IOID_BLUE);

	// ti_lib_gpio_pin_write(DEV_LED_WHITE, 0);
	// ti_lib_gpio_pin_write(DEV_LED_RED, 0);
	// ti_lib_gpio_pin_write(DEV_LED_GREEN, 0);
	// ti_lib_gpio_pin_write(DEV_LED_BLUE, 0);

	etimer_set(&et2, CC26XX_LED_PACK_INTERVAL);

	while(1) {
		static int sel = 0;

		PROCESS_YIELD();

		if(ev == PROCESS_EVENT_TIMER) {
			if(data == &et2) {
				// if( sel == 0) {

				// 	ti_lib_gpio_pin_write(DEV_LED_WHITE, 1);
				// 	ti_lib_gpio_pin_write(DEV_LED_RED, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_GREEN, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_BLUE, 0);

				// 	sel++;

				// } else if( sel == 1) {

				// 	ti_lib_gpio_pin_write(DEV_LED_WHITE, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_RED, 1);
				// 	ti_lib_gpio_pin_write(DEV_LED_GREEN, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_BLUE, 0);

				// 	sel++;

				// } else if( sel == 2) {
					
				// 	ti_lib_gpio_pin_write(DEV_LED_WHITE, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_RED, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_GREEN, 1);
				// 	ti_lib_gpio_pin_write(DEV_LED_BLUE, 0);

				// 	sel++;

				// } else {

				// 	ti_lib_gpio_pin_write(DEV_LED_WHITE, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_RED, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_GREEN, 0);
				// 	ti_lib_gpio_pin_write(DEV_LED_BLUE, 1);

				// 	sel = 0;
				// }

				printf("blinking led devpack\n");
				etimer_set(&et2, CC26XX_LED_PACK_INTERVAL);
			}
		}

	}

	PROCESS_END();

}