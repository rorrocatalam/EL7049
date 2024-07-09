import time
import board
import adafruit_icm20x

# Inicializa I2C y el sensor
i2c = board.I2C()  # usa board.SCL y board.SDA
icm = adafruit_icm20x.ICM20948(i2c)

# Abre el archivo CSV en modo adjuntar
with open('datos_imu.csv', mode='a', newline='') as file:
    while True:
        # Lee los datos de la IMU
        acceleration = icm.acceleration
        gyro = icm.gyro
        magnetic = icm.magnetic
        
        # Formatea los datos como una sola línea de texto
        data_line = f"{acceleration[0]:.2f}, {acceleration[1]:.2f}, {acceleration[2]:.2f}, {gyro[0]:.2f}, {gyro[1]:.2f}, {gyro[2]:.2f}, {magnetic[0]:.2f}, {magnetic[1]:.2f}, {magnetic[2]:.2f}\n"
        
        # Escribe los datos en el archivo CSV
        file.write(data_line)
        file.flush()  # Asegura que los datos se escriban inmediatamente
        
        # Imprime los datos en la consola
        print("Acceleration: X:%.2f, Y: %.2f, Z: %.2f m/s^2" % acceleration)
        print("Gyro: X:%.2f, Y: %.2f, Z: %.2f rads/s" % gyro)
        print("Magnetometer: X:%.2f, Y: %.2f, Z: %.2f uT" % magnetic)
        print("")
        
        # Espera 1 segundo antes de la siguiente lectura
        time.sleep(1)
