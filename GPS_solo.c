#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-1" // Bus I2C
#define GPS_ADDRESS 0x10     // Dirección I2C del GPS XA1110

int main() {
    int file;
    char buffer[128];
    
    // Abrir el bus I2C
    if ((file = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        return 1;
    }

    // Conectarse al dispositivo I2C
    if (ioctl(file, I2C_SLAVE, GPS_ADDRESS) < 0) {
        perror("Failed to connect to the sensor");
        close(file);
        return 1;
    }

    // Leer datos del GPS
    if (read(file, buffer, 128) != 128) {
        perror("Failed to read from the sensor");
        close(file);
        return 1;
    }

    // Procesar los datos del GPS
    // Suponiendo que los datos están en formato NMEA, parseamos la cadena
    float latitude, longitude, altitude;
    sscanf(buffer, "$GPGGA,%*f,%f,N,%f,E,%*d,%*d,%*f,%f", &latitude, &longitude, &altitude);

    printf("Latitud: %.6f\n", latitude);
    printf("Longitud: %.6f\n", longitude);
    printf("Altitud: %.2f m\n", altitude);

    close(file);
    return 0;
}




