#include <hardware.h>
#include <stdlib.h>
#include <variaveis.c>
#include <pulse.c>
#include <ctemp.c>
#include <umodbus.c>

#define DEBUG_MSG(msg) fprintf(PC, msg)
#define DEBUG_DATA(msg,data) fprintf(PC, msg, data)
