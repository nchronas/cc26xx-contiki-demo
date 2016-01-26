#include "contiki.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "button-sensor.h"
#include "board-peripherals.h"
#include "ti-lib.h"

#include <stdio.h>
#include <stdint.h>

#define CC26XX_DEMO_LOOP_INTERVAL       (CLOCK_SECOND * 5)

static struct etimer et;


PROCESS(cc26xx_contiki_demo_process, "cc26xx contiki demo process");
AUTOSTART_PROCESSES(&cc26xx_contiki_demo_process);

PROCESS_THREAD(cc26xx_contiki_demo_process, ev, data)
{

	PROCESS_BEGIN();

	printf("Hello Contiki!!!\nCC26XX contiki demo\n");

    buzzer_start(1000);

	etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);

	while(1) {
		static int counterA = 0 ;

		if(counterA == 0) {
			leds_off(LEDS_RED);
			leds_on(LEDS_GREEN);
		}

		PROCESS_YIELD();

		if(ev == PROCESS_EVENT_TIMER) {
			if(data == &et) {
				counterA++;

				leds_toggle(LEDS_ALL);
				buzzer_stop();

				printf("5 secs passed, time elapsed: %dSEC\n", counterA*5);
				etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);
			}
		} else if(ev == sensors_event) {
			if(data ==  &button_left_sensor) {
				watchdog_reboot();
			} else if(data ==  &button_right_sensor) {
				buzzer_start(500);
			}

		}
	}

	PROCESS_END();

}