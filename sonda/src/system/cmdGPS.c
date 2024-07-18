#include "app/system/cmdGPS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void cmd_start_gps_init(){
	cmd_add("start_gps", start_gps, "", 0);
}

void cmd_save_gps_data_init(){
	cmd_add("save_gps_data", save_gps_data, "", 0);
}

int save_gps_data(char *fmt, char *params, int nparams) {
    const char *filename = "/home/pi/suchai-flight-software/apps/sonda/data/data_gps.csv";
    FILE *file;
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    
    int idx_csv, year, mon, day, hour, min, sec;
    float lat, lon, alt;

    //LOGI("taskSavegpsData", "Guardando datos de GPS!");
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Inicializacion linea vacia
    line[0] = '\0';

    // Se obtiene la ultima linea
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        strcpy(line, buffer);
    }

    fclose(file);

    // Solo se procede en caso de que la linea sea no vacia y diferente a la anterior
    if (line[0] != '\0') {
        // Se obtienen los datos de la linea
        if (sscanf(line, "%d;%f;%f;%f;%d;%d;%d;%d;%d;%d", &idx_csv, &lat, &lon, &alt, &year, &mon, &day, &hour, &min, &sec) == 10) {
            // Se pasa a la base de datos
            gps_data_t gps_sample;
            gps_sample.index = dat_get_system_var(dat_drp_idx_gps);
            gps_sample.timestamp = dat_get_time();

            gps_sample.idx_csv = idx_csv;
            gps_sample.lat = lat;
            gps_sample.lon = lon;
            gps_sample.alt = alt;
            gps_sample.year = year;
            gps_sample.mon = mon;
            gps_sample.day = day;
            gps_sample.hour = hour;
            gps_sample.min = min;
            gps_sample.sec = sec;

            // Se evita que imprima la confirmacion de almacenamiento
            FILE *original_stdout = stdout;
            stdout = fopen("/dev/null", "w");

            dat_add_payload_sample(&gps_sample, gps_sensor);
            
            fclose(stdout);
            stdout = original_stdout;

            return CMD_OK;
        } //else {
            //printf("Error al leer la linea");
        //}
        // Actualizacion de ultima linea
        return CMD_OK;
    }
    return CMD_OK;
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