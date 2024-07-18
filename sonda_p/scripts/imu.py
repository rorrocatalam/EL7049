import time
import board
import adafruit_icm20x

# Inicializacion I2C y sensor
i2c = board.I2C()
icm = adafruit_icm20x.ICM20948(i2c)

# Archivo csv para almacenar informacion
with open('/home/pi/suchai-flight-software/apps/sonda/data/data_imu.csv', mode='w', newline='') as file1:
    with open('/home/pi/suchai-flight-software/apps/sonda/data/data_imu_lora.csv', mode='w', newline='') as file2:
        # Indice para indicar numero de linea
        idx = 1
        while True:
            # Leer datos de la IMU
            acceleration    = icm.acceleration
            gyro            = icm.gyro
            magnetic        = icm.magnetic
            
            # informacion a almacenar
            data_line = f"{idx};{acceleration[0]:.2f};{acceleration[1]:.2f};{acceleration[2]:.2f};{gyro[0]:.2f};{gyro[1]:.2f};{gyro[2]:.2f};{magnetic[0]:.2f};{magnetic[1]:.2f};{magnetic[2]:.2f};\n"
            
            # Almacenamiento de datos
            file1.write(data_line)
            file1.flush()
            file2.write(data_line)
            file2.flush()
            
            # Esperar a siguiente lectura
            time.sleep(1)
            idx += 1
