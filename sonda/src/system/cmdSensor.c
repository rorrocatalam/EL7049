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

void cmd_init_sensor_init(){
	cmd_add("init_sensor", init_sensor, "", 0);
}

int init_sensor(char *fmt, char *params, int nparams){
    pthread_t threads[3];

    // Nombres de los scripts a ejecutar
    char* scripts[] = {
        "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/imu.py",
        "python3 /home/pi/suchai-flight-software/apps/sonda/scripts/gps.py",
        "/home/pi/suchai-flight-software/apps/sonda/scripts/bme",
    };

    // Crear hilos para ejecutar los scripts
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, run_script, (void*)scripts[i])) {
            fprintf(stderr, "Error creando el hilo para el script %s\n", scripts[i]);
            return 1;
        }
    }

    return 0;
}