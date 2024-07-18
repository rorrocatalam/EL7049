#include "app/system/cmdIMU.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void cmd_start_imu_init(){
	cmd_add("start_imu", start_imu, "", 0);
}

void cmd_save_imu_data_init(){
	cmd_add("save_imu_data", save_imu_data, "", 0);
}

int save_imu_data(char *fmt, char *params, int nparams) {
    const char *filename = "/home/pi/suchai-flight-software/apps/sonda/data/data_imu.csv";
    FILE *file;
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];

    //LOGI("taskSaveImuData", "Guardando datos de IMU!");
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
        int idx_csv;
        float acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z, mag_x, mag_y, mag_z;
        // Se obtienen los datos de la linea
        if (sscanf(line, "%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;", &idx_csv, &acc_x, &acc_y, &acc_z, &gyr_x, &gyr_y, &gyr_z, &mag_x, &mag_y, &mag_z) == 10) {
            // Se pasa a la base de datos
            imu_data_t imu_sample;
            imu_sample.index = dat_get_system_var(dat_drp_idx_imu);
            imu_sample.timestamp = dat_get_time();

            imu_sample.acc_x = acc_x;
            imu_sample.acc_y = acc_y;
            imu_sample.acc_z = acc_z;
            imu_sample.gyr_x = gyr_x;
            imu_sample.gyr_y = gyr_y;
            imu_sample.gyr_z = gyr_z;
            imu_sample.mag_x = mag_x;
            imu_sample.mag_y = mag_y;
            imu_sample.mag_z = mag_z;

            // Se evita que imprima la confirmacion de almacenamiento
            FILE *original_stdout = stdout;
            stdout = fopen("/dev/null", "w");

            dat_add_payload_sample(&imu_sample, imu_sensor);
            
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
