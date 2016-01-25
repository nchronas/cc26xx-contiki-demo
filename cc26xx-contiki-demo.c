#include "contiki.h"
#include "ti-lib.h"

#include <stdio.h>
#include <stdint.h>

PROCESS(cc26xx_contiki_demo_process, "cc26xx contiki demo process");
AUTOSTART_PROCESSES(&cc26xx_contiki_demo_process);

PROCESS_THREAD(cc26xx_contiki_demo_process, ev, data)
{

	PROCESS_BEGIN();

	printf("Hello Contiki\nCC26XX contiki demo\n");

	while(1) {

		PROCESS_YIELD();

		printf("From the loop\n");
    
	}

	PROCESS_END();
}