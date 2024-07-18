#include "app/system/cmdBME.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void cmd_start_bme_init(){
	cmd_add("start_bme", start_bme, "", 0);
}

void cmd_save_bme_data_init(){
	cmd_add("save_bme_data", save_bme_data, "", 0);
}

int save_bme_data(char *fmt, char *params, int nparams) {
    const char *filename = "/home/pi/suchai-flight-software/apps/sonda/data/data_bme.csv";
    FILE *file;
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];

    int idx_csv;
    float temp, pres, hum, alt;

    //LOGI("taskSaveBmeData", "Guardando datos de BME!");
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
        if (sscanf(line, "%d;%f;%f;%f;%f;", &idx_csv, &temp, &pres, &hum, &alt) == 5) {
            // Se pasa a la base de datos
            bme_data_t bme_sample;
            bme_sample.index = dat_get_system_var(dat_drp_idx_bme);
            bme_sample.timestamp = dat_get_time();

            bme_sample.idx_csv = idx_csv;
            bme_sample.temp = temp;
            bme_sample.pres = pres;
            bme_sample.hum = hum;
            bme_sample.alt = alt;

            // Se evita que imprima la confirmacion de almacenamiento
            FILE *original_stdout = stdout;
            stdout = fopen("/dev/null", "w");

            dat_add_payload_sample(&bme_sample, bme_sensor);
            
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