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
    


usr = "SONDA"
ser = serial.Serial(
port = "/dev/ttyUSB0",
baudrate = 115200,
parity = serial.PARITY_NONE,
stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS
)
initialize_radio()





#PROGRAMA
with open('/home/pi/suchai-flight-software/apps/sonda/data/data_bme_lora.csv', 'r') as bme:
# Abre el archivo CSV en modo lectura
    with open('/home/pi/suchai-flight-software/apps/sonda/data/data_gps_lora.csv', 'r') as gps:
        with open('/home/pi/suchai-flight-software/apps/sonda/data/data_imu_lora.csv', 'r') as imu:
            while True:
    # Salta a la última línea del archivo
                try:
                    gps.seek(0, 2)  # Mueve el puntero al final del archivo
                    tamaño_archivo1 = gps.tell()  # Obtiene la posición actual (final del archivo)
                    gps.seek(max(tamaño_archivo1 - 1024, 0), 0)  # Salta hasta 1024 bytes antes del final o al principio del archivo

    # Lee las últimas líneas y obtiene la última línea
                    if len(gps.readlines()) == 0:
                        print('GPS sin info')
                        ultima_lineaGPS = 'no hay info'
                    else:
                        ultima_lineaGPS = gps.readlines()[-1]        
    # Imprime la última línea (o haz lo que necesites con ella)

                    imu.seek(0, 2)  # Mueve el puntero al final del archivo
                    tamaño_archivo2 = imu.tell()  # Obtiene la posición actual (final del archivo)
                    imu.seek(max(tamaño_archivo2 - 1024, 0), 0)  # Salta hasta 1024 bytes antes del final o al principio del archivo

    # Lee las últimas líneas y obtiene la última línea
                    ultima_lineaIMU = imu.readlines()[-1]

    # Imprime la última línea (o haz lo que necesites con ella)

                    bme.seek(0, 2)  # Mueve el puntero al final del archivo
                    tamaño_archivo3 = bme.tell()  # Obtiene la posición actual (final del archivo)
                    bme.seek(max(tamaño_archivo3 - 1024, 0), 0)  # Salta hasta 1024 bytes antes del final o al principio del archivo

    # Lee las últimas líneas y obtiene la última línea
                    ultima_lineaBME = bme.readlines()[-1]
                    linea = ultima_lineaGPS + ultima_lineaIMU + ultima_lineaBME
                    send_msg(chr_to_hex(linea))
    # Imprime la última línea
    # 
                except:
                    print('Ha ocurrido un error')
                    send_msg(chr_to_hex('Ha ocurrido un error'))
                time.sleep(5)
