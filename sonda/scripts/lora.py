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
    with open(filename, 'r') as file:
        lines = file.readlines()
        if lines:
            return lines[-1].strip()
        else:
            return " "

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
    line1 = read_last_line(file_imu)
    line2 = read_last_line(file_bme)
    line3 = read_last_line(file_gps)
    linea = f"{line1}\n{line2}\n{line3}"
    # Envia cada 5 segundos
    send_msg(chr_to_hex(linea))
    time.sleep(5)