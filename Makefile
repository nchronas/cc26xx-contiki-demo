DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = cc26xx-contiki-demo

all: $(CONTIKI_PROJECT)

CONTIKI = ~/contiki
include $(CONTIKI)/Makefile.include