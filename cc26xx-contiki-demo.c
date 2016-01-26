#include "contiki.h"
#include "dev/leds.h"
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

	leds_init();

	printf("Hello Contiki!!!\nCC26XX contiki demo\n");

	for(int i = 0; i < 3; i++) {
		
		leds_on(LEDS_ALL);

		clock_delay(400);

		leds_off(LEDS_ALL);

		clock_delay(400);
	}

	etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);

	leds_on(LEDS_RED);
	leds_off(LEDS_YELLOW);

	while(1) {
		static int counterA = 0 ;

		PROCESS_YIELD();

		if(ev == PROCESS_EVENT_TIMER) {
			if(data == &et) {
				counterA++;

				leds_toggle(LEDS_YELLOW);
				leds_toggle(LEDS_RED);

				for(int i = 0; i < 3; i++) {
		
					leds_on(LEDS_ALL);

					clock_delay(400);

					leds_off(LEDS_ALL);

					clock_delay(400);
				}

				printf("5 secs passed, time elapsed: %dSEC\n", counterA*5);
				etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);
			}
		}
	}

	PROCESS_END();

}