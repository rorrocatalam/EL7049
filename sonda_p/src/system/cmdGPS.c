#include "app/system/cmdGPS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void cmd_start_gps_init(){
	cmd_add("start_gps", start_gps, "", 0);
}

int start_gps(char *fmt, char *params, int nparams){
    const char *com = "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/gps.py";
    int res = system(com);

    // Verificar si el comando se ejecutó correctamente
    if (res == -1) {
        perror("Error al ejecutar gps.py");
        return CMD_ERROR;
    } else {
        return CMD_OK;
    }
}