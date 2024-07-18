#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <time.h>

#define BME280_ADDRESS 0x76
#define BME280_REGISTER_CHIPID 0xD0
#define BME280_REGISTER_CONTROL_HUM 0xF2
#define BME280_REGISTER_CONTROL 0xF4
#define BME280_REGISTER_TEMPDATA 0xFA
#define BME280_REGISTER_PRESSUREDATA 0xF7
#define BME280_REGISTER_HUMIDDATA 0xFD

int file;
int32_t t_fine; // Global variable for t_fine

// Compensation parameters
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
uint8_t dig_H1;
int16_t dig_H2, dig_H3;
int16_t dig_H4, dig_H5, dig_H6;

// Function to write a byte to a register on the BME280 sensor
void writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    if (write(file, buf, 2) != 2) {
        perror("Error writing to register");
        exit(1);
    }
}

// Function to read bytes from a specific register on the BME280 sensor
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
    uint8_t buf[26];
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

    readBytes(0xA1, &dig_H1, 1);
    readBytes(0xE1, buf, 7);
    dig_H2 = (buf[1] << 8) | buf[0];
    dig_H3 = buf[2];
    dig_H4 = (buf[3] << 4) | (buf[4] & 0xF);
    dig_H5 = (buf[5] << 4) | ((buf[4] >> 4) & 0xF);
    dig_H6 = (int8_t)buf[6];
}

// Function to initialize the BME280 sensor
void bme280Init() {
    if ((file = open("/dev/i2c-1", O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }
    if (ioctl(file, I2C_SLAVE, BME280_ADDRESS) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        exit(1);
    }
    uint8_t chipid;
    readBytes(BME280_REGISTER_CHIPID, &chipid, 1);
    if (chipid != 0x60) {
        printf("Error: BME280 not found\n");
        exit(1);
    }
    readCompensationParameters();
    // Configure the sensor
    writeRegister(BME280_REGISTER_CONTROL_HUM, 0x01); // Set humidity oversampling
    writeRegister(BME280_REGISTER_CONTROL, 0x3F); // Set oversampling to max for temperature and pressure
}

// Function to read temperature from the BME280 sensor
double bme280ReadTemperature() {
    int32_t var1, var2, adc_T;
    uint8_t buf[3];
    readBytes(BME280_REGISTER_TEMPDATA, buf, 3);
    adc_T = (int32_t)(((uint32_t)buf[0] << 12) | ((uint32_t)buf[1] << 4) | ((uint32_t)buf[2] >> 4));

    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    double T = (t_fine * 5 + 128) >> 8;
    return T / 100.0;
}

// Function to read pressure from the BME280 sensor
double bme280ReadPressure() {
    int64_t var1, var2, p;
    int32_t adc_P;
    uint8_t buf[3];
    readBytes(BME280_REGISTER_PRESSUREDATA, buf, 3);
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

// Function to read humidity from the BME280 sensor
double bme280ReadHumidity() {
    int32_t adc_H;
    uint8_t buf[2];
    readBytes(BME280_REGISTER_HUMIDDATA, buf, 2);
    adc_H = (int32_t)((((uint32_t)buf[0] << 8) | ((uint32_t)buf[1])));

    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    double h = (v_x1_u32r >> 12);
    return h / 1024.0;
}

int main() {
    bme280Init();
    FILE *csv_file1 = fopen("/home/pi/suchai-flight-software/apps/sonda/data/data_bme.csv", "w"); // Abrir archivo en modo escritura
    if(csv_file1 == NULL){
      perror("Failed to open file");
      return 1;
    }
    FILE *csv_file2 = fopen("/home/pi/suchai-flight-software/apps/sonda/data/data_bme_lora.csv", "w"); // Abrir archivo en modo escritura
    if(csv_file2 == NULL){
      perror("Failed to open file");
      return 1;
    }
    //fprintf(csv_file1, "Temperature,Pressure,Humidity,Altitude\n");
    int idx_csv = 1;
    while (1) {
        double temperature = bme280ReadTemperature();
        double pressure = bme280ReadPressure();
        double humidity = bme280ReadHumidity();
        double P0 = 1013; /*presion atmosferica nivel mar*/
        double altitude = 44330 * (1-pow(pressure/P0,1/5.255));

        fprintf(csv_file1 , "%d;%.2f;%.2f;%.2f;%.2f;\n", idx_csv, temperature, pressure, humidity, altitude);
        fflush(csv_file1 ); // Ensure data is written to the file immediately
        fprintf(csv_file2 , "%d;%.2f;%.2f;%.2f;%.2f;\n", idx_csv, temperature, pressure, humidity, altitude);
        fflush(csv_file2 ); // Ensure data is written to the file immediately


        //printf("Temperature: %.2f °C\n", temperature);
        //printf("Pressure: %.2f hPa\n", pressure);
        //printf("Altitude: %.2f m\n",altitude);
        //printf("Humidity: %.2f %%\n", humidity);
        idx_csv++;
        sleep(1);
    }
    return 0;
}

