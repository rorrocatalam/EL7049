import time
import board
import adafruit_icm20x

i2c = board.I2C()
icm = adafruit_icm20x.ICM20948(i2c)

output_file = "sensor_data.txt"  # Nombre del archivo donde se guardarán los datos

while True:
    acceleration = icm.acceleration
    gyro = icm.gyro
    magnetometer = icm.magnetic
    
    with open(output_file, "a") as file:
        file.write(f"Acceleration: X:{acceleration[0]:.2f}, Y: {acceleration[1]:.2f}, Z: {acceleration[2]:.2f} m/s^2\n")
        file.write(f"Gyro X:{gyro[0]:.2f}, Y: {gyro[1]:.2f}, Z: {gyro[2]:.2f} rads/s\n")
        file.write(f"Magnetometer X:{magnetometer[0]:.2f}, Y: {magnetometer[1]:.2f}, Z: {magnetometer[2]:.2f} uT\n")
        file.write("\n")

    time.sleep(1)  # Espera de 1 segundo antes de tomar la siguiente lectura
