#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

// Dirección I2C del IMC-20948
#define IMU_ADDRESS 0x69 /*hay que arreglar esta wea**/

// Registro de configuraciones y datos de la IMU
#define PWR_MGMT_1 0x06
/*#define ACCEL_XOUT_H 0x2D
#define GYRO_XOUT_H 0x33
#define MAG_XOUT_L 0x10*/
#define ACCEL_CONFIG 0x20
//#define GYRO_CONFIG 0x11
#define WHO_AM_I 0x00

#define ACCEL_XOUT_H 0x2D
#define ACCEL_XOUT_L 0x2E
#define GYRO_XOUT_H 0x33
#define GYRO_XOUT_L 0x34

#define ACCEL_YOUT_H 0x2F
#define ACCEL_YOUT_L 0x30
#define GYRO_YOUT_H 0x35
#define GYRO_YOUT_L 0x36

#define ACCEL_ZOUT_H 0x31
#define ACCEL_ZOUT_L 0x32
#define GYRO_ZOUT_H 0x37
#define GYRO_ZOUT_L 0x38

void initializeIMU(int fd) {
    // Sacar al IMU del modo de sueño
    i2cWriteByteData(fd, PWR_MGMT_1, 0x01);

    // Configurar el rango del acelerómetro a ±4g
    i2cWriteByteData(fd, ACCEL_CONFIG, 0x02);

    // Configurar el rango del giroscopio a ±500 grados/segundo
   // i2cWriteByteData(fd, GYRO_CONFIG, 0x01);
}

/*hay que ver si estan bien las operaciones*/
void readAccelerometer(int fd, int16_t *accelX, int16_t *accelY, int16_t *accelZ) {
    *accelX = ((i2cReadByteData(fd, ACCEL_XOUT_H) << 8) | i2cReadByteData(fd, ACCEL_XOUT_L))/8.192;;
    *accelY = ((i2cReadByteData(fd, ACCEL_YOUT_H ) << 8) | i2cReadByteData(fd, ACCEL_YOUT_L ))8.192;;
    *accelZ = ((i2cReadByteData(fd, ACCEL_ZOUT_H ) << 8) | i2cReadByteData(fd, ACCEL_ZOUT_L ))/8.192;;
}

void readGyroscope(int fd, int16_t *gyroX, int16_t *gyroY, int16_t *gyroZ) {
    *gyroX = ((i2cReadByteData(fd, GYRO_XOUT_H) << 8) | i2cReadByteData(fd, GYRO_XOUT_L))/8.192;
    *gyroY = ((i2cReadByteData(fd, GYRO_YOUT_H) << 8) | i2cReadByteData(fd, GYRO_YOUT_L ))/8.192;
    *gyroZ = ((i2cReadByteData(fd, GYRO_ZOUT_H ) << 8) | i2cReadByteData(fd, GYRO_ZOUT_L ))/8.192;
}

/*void readMagnetometer(int fd, int16_t *magX, int16_t *magY, int16_t *magZ) {
    *magX = (i2cReadByteData(fd, MAG_XOUT_L) | (i2cReadByteData(fd, MAG_XOUT_L + 1) << 8));
    *magY = (i2cReadByteData(fd, MAG_XOUT_L + 2) | (i2cReadByteData(fd, MAG_XOUT_L + 3) << 8));
    *magZ = (i2cReadByteData(fd, MAG_XOUT_L + 4) | (i2cReadByteData(fd, MAG_XOUT_L + 5) << 8));
}*/

int main() {
    if (gpioInitialise() < 0) {
        printf("Error al inicializar pigpio\n");
        return -1;
    }

    int fd = i2cOpen(1, IMU_ADDRESS, 0);
    if (fd < 0) {
        printf("Error al abrir el dispositivo I2C\n");
        return -1;
    }

    // Verificar si la IMU está presente
    int whoAmI = i2cReadByteData(fd, WHO_AM_I);
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
       // readMagnetometer(fd, &magX, &magY, &magZ);

        // Obtener la hora del sistema
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);

        // Mostrar datos con la hora
        printf("Hora: %02d:%02d:%02d\n", local->tm_hour, local->tm_min, local->tm_sec);
        printf("Accel X: %d, Accel Y: %d, Accel Z: %d\n", accelX, accelY, accelZ);
        printf("Gyro X: %d, Gyro Y: %d, Gyro Z: %d\n", gyroX, gyroY, gyroZ);
        //printf("Mag X: %d, Mag Y: %d, Mag Z: %d\n", magX, magY, magZ);

        usleep(100000); // Esperar 100 ms
    }

    i2cClose(fd);
    gpioTerminate();

    return 0;
}
