#include "app/system/cmdSensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Función para ejecutar un script
void* run_script(void* arg) {
    char* script_name = (char*)arg;
    char command[256];
    snprintf(command, sizeof(command), "%s", script_name);
    system(command);
    return NULL;
}

void cmd_start_sensor_init(){
	cmd_add("start_sensor", start_sensor, "", 0);
}


int start_sensor(char *fmt, char *params, int nparams){
    pthread_t threads[4];

    // Nombres de los scripts a ejecutar
    char* scripts[] = {
        "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/imu.py",
        "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/gps.py",
        "/home/pi/suchai-flight-software/apps/sonda/scripts/bme",
        "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/lora.py",
    };

    // Crear hilos para ejecutar los scripts
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, run_script, (void*)scripts[i])) {
            fprintf(stderr, "Error creando el hilo para el script %s\n", scripts[i]);
            return 1;
        }
    }

    return 0;
}
