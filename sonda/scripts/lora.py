import os
import time
import serial
import codecs

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
    
def read_last_line(filename):
    with open(filename, 'rb') as file:
        file.seek(0, os.SEEK_END)
        position = file.tell()
        buffer = bytearray()
        
        while position >= 0:
            file.seek(position)
            byte = file.read(1)
            if byte == b'\n' and buffer:
                break
            buffer.extend(byte)
            position -= 1
        
        last_line = buffer[::-1].decode().strip()
        return last_line if last_line else ""

usr = "SONDA"
ser = serial.Serial(
port = "/dev/ttyUSB0",
baudrate = 115200,
parity = serial.PARITY_NONE,
stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS
)
initialize_radio()

file_imu = "/home/pi/suchai-flight-software/apps/sonda/data/data_imu_lora.csv"
file_bme = "/home/pi/suchai-flight-software/apps/sonda/data/data_bme_lora.csv"
file_gps = "/home/pi/suchai-flight-software/apps/sonda/data/data_gps_lora.csv"

while True:
    # IMU
    line_imu = read_last_line(file_imu)
    if line_imu != "":
        values_imu = line_imu.split(";")
        idx_csv_imu = int(values_imu[0])
        acc_x = float(values_imu[1])
        acc_y = float(values_imu[2])
        acc_z = float(values_imu[3])
        gyr_x = float(values_imu[4])
        gyr_y = float(values_imu[5])
        gyr_z = float(values_imu[6])
        mag_x = float(values_imu[7])
        mag_y = float(values_imu[8])
        mag_z = float(values_imu[9])
        line_imu = f"i={idx_csv_imu}; acc=({acc_x},{acc_y},{acc_z}); gyr=({gyr_x},{gyr_y},{gyr_z}); mag=({mag_x},{mag_y},{mag_z})"
    else:
        line_imu = "No IMU data"

    # BME
    line_bme = read_last_line(file_bme)
    if line_bme != "":
        values_bme = line_bme.split(";")
        idx_csv_bme = int(values_bme[0])
        temp = float(values_bme[1])
        pres = float(values_bme[2])
        hum = float(values_bme[3])
        alt = float(values_bme[4])
        line_bme = f"i={idx_csv_bme}; temp={temp}; pres={pres}; hum={hum}; alt={alt}"
    else:
        line_imu = "No BME data"
        
    # GPS
    line_gps = read_last_line(file_gps)
    if line_gps != "":
        values_gps = line_gps.split(";")
        idx_csv_gps = int(values_gps[0])
        lat = float(values_gps[1])
        lon = float(values_gps[2])
        alt = float(values_gps[3])
        year = int(values_gps[4])
        mon = int(values_gps[5])
        day = int(values_gps[6])
        hour = int(values_gps[7])
        min = int(values_gps[8])
        sec = int(values_gps[9])
        line_gps = f"i={idx_csv_gps}; lat={lat}; lon={lon}; alt={alt}; {year}/{mon}/{day} {hour}:{min}:{sec}"
    else:
        line_imu = "No GPS data"

    # Mensaje a enviar
    linea = f"{line_imu}\n{line_bme}\n{line_gps}"
    # Envia cada 5 segundos
    send_msg(chr_to_hex(linea))
    time.sleep(5)