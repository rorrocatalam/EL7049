#include "app/system/cmdBME.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void cmd_start_bme_init(){
	cmd_add("start_bme", start_bme, "", 0);
}

int start_bme(char *fmt, char *params, int nparams){
    const char *com = "/home/pi/suchai-flight-software/apps/sonda/scripts/bme";
    int res = system(com);

    // Verificar si el comando se ejecutó correctamente
    if (res == -1) {
        perror("Error al ejecutar bme");
        return CMD_ERROR;
    } else {
        return CMD_OK;
    }
}