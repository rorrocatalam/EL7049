#include <stdio.h>
#include <wiringPiI2C.h> /*sudo apt-get install wiringpi*/

#include <unistd.h>
#include <time.h>

// Dirección I2C del IMC-20948
#define IMU_ADDRESS 0x68

// Registro de configuraciones y datos de la IMU
#define PWR_MGMT_1 0x06
#define ACCEL_XOUT_H 0x2D
#define GYRO_XOUT_H 0x33
#define MAG_XOUT_L 0x10
#define ACCEL_CONFIG 0x14
#define GYRO_CONFIG 0x11
#define WHO_AM_I 0x00

void initializeIMU(int fd) {
    // Sacar al IMU del modo de sueño
    wiringPiI2CWriteReg8(fd, PWR_MGMT_1, 0x01);

    // Configurar el rango del acelerómetro a ±4g
    wiringPiI2CWriteReg8(fd, ACCEL_CONFIG, 0x02);

    // Configurar el rango del giroscopio a ±500 grados/segundo
    wiringPiI2CWriteReg8(fd, GYRO_CONFIG, 0x01);

    // Configurar el magnetómetro (si es necesario, ver documentación específica)
    // Aquí deberías configurar los registros correspondientes para activar el magnetómetro
}

void readAccelerometer(int fd, int16_t *accelX, int16_t *accelY, int16_t *accelZ) {
    *accelX = (wiringPiI2CReadReg8(fd, ACCEL_XOUT_H) << 8) | wiringPiI2CReadReg8(fd, ACCEL_XOUT_H + 1);
    *accelY = (wiringPiI2CReadReg8(fd, ACCEL_XOUT_H + 2) << 8) | wiringPiI2CReadReg8(fd, ACCEL_XOUT_H + 3);
    *accelZ = (wiringPiI2CReadReg8(fd, ACCEL_XOUT_H + 4) << 8) | wiringPiI2CReadReg8(fd, ACCEL_XOUT_H + 5);
}

void readGyroscope(int fd, int16_t *gyroX, int16_t *gyroY, int16_t *gyroZ) {
    *gyroX = (wiringPiI2CReadReg8(fd, GYRO_XOUT_H) << 8) | wiringPiI2CReadReg8(fd, GYRO_XOUT_H + 1);
    *gyroY = (wiringPiI2CReadReg8(fd, GYRO_XOUT_H + 2) << 8) | wiringPiI2CReadReg8(fd, GYRO_XOUT_H + 3);
    *gyroZ = (wiringPiI2CReadReg8(fd, GYRO_XOUT_H + 4) << 8) | wiringPiI2CReadReg8(fd, GYRO_XOUT_H + 5);
}

void readMagnetometer(int fd, int16_t *magX, int16_t *magY, int16_t *magZ) {
    *magX = (wiringPiI2CReadReg8(fd, MAG_XOUT_L) | (wiringPiI2CReadReg8(fd, MAG_XOUT_L + 1) << 8));
    *magY = (wiringPiI2CReadReg8(fd, MAG_XOUT_L + 2) | (wiringPiI2CReadReg8(fd, MAG_XOUT_L + 3) << 8));
    *magZ = (wiringPiI2CReadReg8(fd, MAG_XOUT_L + 4) | (wiringPiI2CReadReg8(fd, MAG_XOUT_L + 5) << 8));
}

int main() {
    int fd = wiringPiI2CSetup(IMU_ADDRESS);
    if (fd == -1) {
        printf("Error al inicializar I2C\n");
        return -1;
    }

    // Verificar si la IMU está presente
    int whoAmI = wiringPiI2CReadReg8(fd, WHO_AM_I);
    if (whoAmI != 0xEA) {
        printf("IMU no encontrada\n");
        return -1;
    }

    // Inicializar la IMU
    initializeIMU(fd);

    while (1) {
        int16_t accelX, accelY, accelZ;
        int16_t gyroX, gyroY, gyroZ;
        int16_t magX, magY, magZ;

        // Leer datos del acelerómetro, giroscopio y magnetómetro
        readAccelerometer(fd, &accelX, &accelY, &accelZ);
        readGyroscope(fd, &gyroX, &gyroY, &gyroZ);
        readMagnetometer(fd, &magX, &magY, &magZ);

        // Obtener la hora del sistema
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);

        // Mostrar datos con la hora
        printf("Hora: %02d:%02d:%02d\n", local->tm_hour, local->tm_min, local->tm_sec);
        printf("Accel X: %d, Accel Y: %d, Accel Z: %d\n", accelX, accelY, accelZ);
        printf("Gyro X: %d, Gyro Y: %d, Gyro Z: %d\n", gyroX, gyroY, gyroZ);
        printf("Mag X: %d, Mag Y: %d, Mag Z: %d\n", magX, magY, magZ);

        usleep(100000); // Esperar 100 ms
    }

    return 0;
}


