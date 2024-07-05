from smbus2 import SMBus
import time
from bmp280 import BMP280 #pip install bmp280

bus = SMBus(1)

bmp_280 = BMP280(i2c_dev = bus)

while True:
    temperatura =bmp_280.get_temperature()
    presion = bmp_280.get_pressure()
    print(f"{temperatura:05.2f}*C {presion:05.2f}hPa")
    time.sleep(1)
