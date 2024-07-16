#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "app/system/taskSaveData.h"

#define BUFFER_SIZE 1024

void taskSaveImuData(void *params) {
    const char *filename = "/home/pi/suchai-flight-software/apps/sonda/data/data_imu.csv";
    FILE *file;
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    char line_prev[BUFFER_SIZE] = "";

    int idx_csv;
    float acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z, mag_x, mag_y, mag_z;

    while (1) {
        osDelay(1000);
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
        if (line[0] != '\0' && strcmp(line, line_prev) != 0) {
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
            } else {
                printf("Error al leer la linea");
            }
            // Actualizacion de ultima linea
            strcpy(line_prev, line);
        }
    }
}

void taskSaveBmeData(void *params) {
    const char *filename = "/home/pi/suchai-flight-software/apps/sonda/data/data_bme.csv";
    FILE *file;
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    char line_prev[BUFFER_SIZE] = "";

    int idx_csv;
    float temp, pres, hum, alt;

    while (1) {
        osDelay(1000);
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
        if (line[0] != '\0' && strcmp(line, line_prev) != 0) {
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
            } else {
                printf("Error al leer la linea");
            }
            // Actualizacion de ultima linea
            strcpy(line_prev, line);
        }
    }
}

void taskSaveGpsData(void *params) {
    const char *filename = "/home/pi/suchai-flight-software/apps/sonda/data/data_gps.csv";
    FILE *file;
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    char line_prev[BUFFER_SIZE] = "";

    int idx_csv, year, mon, day, hour, min;
    float lat, lon, alt;

    while (1) {
        osDelay(1000);
        //LOGI("taskSaveBmeData", "Guardando datos de GPS!");
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
        if (line[0] != '\0' && strcmp(line, line_prev) != 0) {
            // Se obtienen los datos de la linea
            if (sscanf(line, "%d;%f;%f;%f;%d;%d;%d;%d;%d;", &idx_csv, &lat, &lon, &alt, &year, &mon, &day, &hour, &min) == 9) {
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

                // Se evita que imprima la confirmacion de almacenamiento
                FILE *original_stdout = stdout;
                stdout = fopen("/dev/null", "w");

                dat_add_payload_sample(&gps_sample, gps_sensor);
                
                fclose(stdout);
                stdout = original_stdout;
            } else {
                printf("Error al leer la linea");
            }
            // Actualizacion de ultima linea
            strcpy(line_prev, line);
        }
    }
}