import serial
import datetime
import codecs
import time
import board
import adafruit_icm20x

i2c = board.I2C()
icm = adafruit_icm20x.ICM20948(i2c)

uart = None
gps  = None

# Inicializaciones
serial_port = ""
baud_rate = 115200
freq  =  915
mod = "SF9"
band_width = 125
tx_pr = 8
rx_pr = 8
power = 22
rf_conf_str = "AT+TEST=RFCFG,{},{},{},{},{},{},OFF,OFF,OFF\n".format(freq, mod, band_width, tx_pr, rx_pr, power)
ser = None
send = True
usr = ""

def initialize_radio():  #TEST passed
    ser.write("AT+MODE=TEST\n".encode())
    time.sleep(0.5)
    print(ser.readline().decode())
    ser.write(rf_conf_str.encode())
    time.sleep(0.5)
    print(ser.readline().decode())

def chr_to_hex(string):
    return codecs.encode(string.encode(),"hex").decode()

def send_msg(message):
    ser.write("AT+TEST=TXLRPKT,\"{}\"\n".format(message).encode())
    time.sleep(0.5)

def main(n_msr, interval_time):
    global usr,ser
    usr = "IMU"
    ser = serial.Serial(
    port = "/dev/ttyUSB0",
    baudrate = 115200,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS
    )
    initialize_radio()

    n_msr = int(n_msr)
    interval_time = float(interval_time)
    print("El intervalo de tiempo es:",interval_time)
    print("La cantidad de iteraciones es:",n_msr)
    i = 0
    output_file = "Data_IMU.txt"

    while True:
        hora = datetime.datetime.now()
        time.sleep(1)

        acceleration = icm.acceleration
        gyro = icm.gyro
        magnetometer = icm.magnetic
        
        with open(output_file, "a") as file:
            hora = datetime.datetime.now()
            string1 = f"Acceleration: X:{acceleration[0]:.2f}, Y: {acceleration[1]:.2f}, Z: {acceleration[2]:.2f} m/s^2\n"
            string2 = f"Gyro X:{gyro[0]:.2f}, Y: {gyro[1]:.2f}, Z: {gyro[2]:.2f} rads/s\n"
            string3 = f"Magnetometer X:{magnetometer[0]:.2f}, Y: {magnetometer[1]:.2f}, Z: {magnetometer[2]:.2f} uT\n"
            mensaje = f"[{hora}]{string1}{string2}{string3}"
            file.write(mensaje + "\n")

            if i%5==0:
                send_msg(chr_to_hex(string1))
                send_msg(chr_to_hex(string2))
                send_msg(chr_to_hex(string3))
                print(mensaje)
            i+=1
            
if __name__ == "__main__":
    main(100,1)
