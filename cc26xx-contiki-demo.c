#include "contiki.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "button-sensor.h"
#include "board-peripherals.h"
#include "sys/mt.h"
#include "ti-lib.h"

#include <stdio.h>
#include <stdint.h>

#define CC26XX_DEMO_LOOP_INTERVAL       (CLOCK_SECOND / 2)

static struct etimer et;
static int counterA = 0 ;

PROCESS(button_process, "button process");

PROCESS(cc26xx_contiki_demo_process, "cc26xx contiki demo process");
AUTOSTART_PROCESSES(&cc26xx_contiki_demo_process, &button_process);


PROCESS_THREAD(cc26xx_contiki_demo_process, ev, data)
{
	static struct mt_thread thread;

	PROCESS_BEGIN();

	
	mt_start(&thread, thread_entry, NULL);

	printf("Hello Contiki!!!\nCC26XX contiki demo\n");

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
				

				if(counterA % 2) {
					printf("Starting thread\n");
					mt_exec(&thread);
				}

				printf("100msecs passed, time elapsed: %fs\n", counterA*0.5);
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
			watchdog_reboot();
		} else if(ev == sensors_event && data ==  &button_right_sensor) {
			printf("Right button pushed, time: %fs\n", counterA*0.5);
			if(buzzer_state()) {
				buzzer_stop();
			} else {
				buzzer_start(1000);
			}
		}

	}

	PROCESS_END();

}

void thread_entry(void *data)
{
	int counterB = 0;

	whiler(1) {
		printf("Looping in thread_entry %d\n", counterB);
		counterB++;
		mt_yield();
	}
}