#include "contiki.h"
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

	etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);

	while(1) {

		PROCESS_YIELD();

		if(ev == PROCESS_EVENT_TIMER) {
			if(data == &et) {

				printf("5 secs passed\n");
				etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);
			}
		}
	}

	PROCESS_END();

}