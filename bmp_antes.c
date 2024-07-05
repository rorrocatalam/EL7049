#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>

#define BMP280_ADDRESS 0x76
#define BMP280_REGISTER_CHIPID 0xD0
#define BMP280_REGISTER_CONTROL 0xF4
#define BMP280_REGISTER_TEMPDATA 0xFA
#define BMP280_REGISTER_PRESSUREDATA 0xF7

int file;
int32_t t_fine; // Global variable for t_fine

// Compensation parameters
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

// Function to write a byte to a register on the BMP280 sensor
void writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    if (write(file, buf, 2) != 2) {
        perror("Error writing to register");
        exit(1);
    }
}

// Function to read bytes from a specific register on the BMP280 sensor
void readBytes(uint8_t reg, uint8_t *buf, uint8_t len) {
    if (write(file, &reg, 1) != 1) {
        perror("Error writing to register");
        exit(1);
    }
    if (read(file, buf, len) != len) {
        perror("Error reading from register");
        exit(1);
    }
}

// Function to read compensation parameters
void readCompensationParameters() {
    uint8_t buf[24];
    readBytes(0x88, buf, 24);
    dig_T1 = (buf[1] << 8) | buf[0];
    dig_T2 = (buf[3] << 8) | buf[2];
    dig_T3 = (buf[5] << 8) | buf[4];
    dig_P1 = (buf[7] << 8) | buf[6];
    dig_P2 = (buf[9] << 8) | buf[8];
    dig_P3 = (buf[11] << 8) | buf[10];
    dig_P4 = (buf[13] << 8) | buf[12];
    dig_P5 = (buf[15] << 8) | buf[14];
    dig_P6 = (buf[17] << 8) | buf[16];
    dig_P7 = (buf[19] << 8) | buf[18];
    dig_P8 = (buf[21] << 8) | buf[20];
    dig_P9 = (buf[23] << 8) | buf[22];
}

// Function to initialize the BMP280 sensor
void bmp280Init() {
    if ((file = open("/dev/i2c-1", O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }
    if (ioctl(file, I2C_SLAVE, BMP280_ADDRESS) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        exit(1);
    }
    uint8_t chipid;
    readBytes(BMP280_REGISTER_CHIPID, &chipid, 1);
    if (chipid != 0x60) {
        printf("Error: BMP280 not found\n");
        exit(1);
    }
    readCompensationParameters();
    // Configure the sensor
    writeRegister(BMP280_REGISTER_CONTROL, 0x3F); // Set oversampling to max for temperature and pressure
}

// Function to read temperature from the BMP280 sensor
double bmp280ReadTemperature() {
    int32_t var1, var2, adc_T;
    uint8_t buf[3];
    readBytes(BMP280_REGISTER_TEMPDATA, buf, 3);
    adc_T = (int32_t)(((uint32_t)buf[0] << 12) | ((uint32_t)buf[1] << 4) | ((uint32_t)buf[2] >> 4));

    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    double T = (t_fine * 5 + 128) >> 8;
    return T / 100.0;
}

// Function to read pressure from the BMP280 sensor
double bmp280ReadPressure() {
    int64_t var1, var2, p;
    int32_t adc_P;
    uint8_t buf[3];
    readBytes(BMP280_REGISTER_PRESSUREDATA, buf, 3);
    adc_P = (int32_t)(((uint32_t)buf[0] << 12) | ((uint32_t)buf[1] << 4) | ((uint32_t)buf[2] >> 4));

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = ((((int64_t)1) << 47) + var1) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0) {
        return 0; // Avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    return (double)p / 256.0 / 100.0;
}

int main() {
    bmp280Init();
    while (1) {
        double temperature = bmp280ReadTemperature();
        double pressure = bmp280ReadPressure();
        double g = 1025;   //1025
        double height = 44330*(1-pow(pressure/g,1/5.255));
        printf("Temperature: %.2f °C\n", temperature);
        printf("Pressure: %.2f hPa\n", pressure);
//        printf("height %.2f m\n",height*1000);
        sleep(1);
    }
    return 0;
}
