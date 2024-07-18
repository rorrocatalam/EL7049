#include "app/system/cmdLora.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void cmd_start_lora_init(){
	cmd_add("start_lora", start_lora, "", 0);
}

int start_lora(char *fmt, char *params, int nparams){
    const char *com = "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/lora.py";
    int res = system(com);

    // Verificar si el comando se ejecutó correctamente
    if (res == -1) {
        perror("Error al ejecutar lora.py");
        return CMD_ERROR;
    } else {
        return CMD_OK;
    }
}