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
    bme_data_t bme_sample;
    bme_sample.index = dat_get_system_var(dat_drp_idx_bme);
    bme_sample.timestamp = dat_get_time();
    printf("%u\n",bme_sample.index);
    float a = 1.0;
    float b = 1.0;
    float c = 1.0;
    float d = 1.0;
    bme_sample.temp = a;
    bme_sample.pres = b;
    bme_sample.hum = c;
    bme_sample.alt = d;

    int rc = dat_add_payload_sample(&bme_sample, bme_sensor);
    return rc != -1 ? CMD_OK : CMD_ERROR;
}
/*
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
*/