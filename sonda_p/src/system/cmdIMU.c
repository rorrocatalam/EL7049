#include "app/system/cmdIMU.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void cmd_start_imu_init(){
	cmd_add("start_imu", start_imu, "", 0);
}

int start_imu(char *fmt, char *params, int nparams){
    const char *com = "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/imu.py";
    int res = system(com);

    // Verificar si el comando se ejecutó correctamente
    if (res == -1) {
        perror("Error al ejecutar imu.py");
        return CMD_ERROR;
    } else {
        return CMD_OK;
    }
}
