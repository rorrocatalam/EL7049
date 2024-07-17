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
with open('csv3.csv', 'r') as archivo3:
# Abre el archivo CSV en modo lectura
    with open('csv1.csv', 'r') as archivo1:
        with open('csv2.csv', 'r') as archivo2:
            while True:
    # Salta a la última línea del archivo
                archivo1.seek(0, 2)  # Mueve el puntero al final del archivo
                tamaño_archivo1 = archivo1.tell()  # Obtiene la posición actual (final del archivo)
                archivo1.seek(max(tamaño_archivo1 - 1024, 0), 0)  # Salta hasta 1024 bytes antes del final o al principio del archivo

    # Lee las últimas líneas y obtiene la última línea
                ultima_linea1 = archivo1.readlines()[-1]
                
    # Imprime la última línea (o haz lo que necesites con ella)

                archivo2.seek(0, 2)  # Mueve el puntero al final del archivo
                tamaño_archivo2 = archivo2.tell()  # Obtiene la posición actual (final del archivo)
                archivo2.seek(max(tamaño_archivo2 - 1024, 0), 0)  # Salta hasta 1024 bytes antes del final o al principio del archivo

    # Lee las últimas líneas y obtiene la última línea
                ultima_linea2 = archivo2.readlines()[-1]

    # Imprime la última línea (o haz lo que necesites con ella)

                archivo3.seek(0, 2)  # Mueve el puntero al final del archivo
                tamaño_archivo3 = archivo3.tell()  # Obtiene la posición actual (final del archivo)
                archivo3.seek(max(tamaño_archivo3 - 1024, 0), 0)  # Salta hasta 1024 bytes antes del final o al principio del archivo

    # Lee las últimas líneas y obtiene la última línea
                ultima_linea3 = archivo3.readlines()[-1]
                linea = ultima_linea1 + ultima_linea2 + ultima_linea3
                send_msg(chr_to_hex(linea))
    # Imprime la última línea (o haz lo que necesites con ella)
                time.sleep(5)