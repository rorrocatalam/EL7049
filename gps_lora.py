import adafruit_gps
import serial
import datetime
import time
import codecs

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

def initialize_radio():  #tEST passed
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
    usr = "GPS"
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
    now = datetime.datetime.now()
    name = now.strftime("%m-%d-%H-%M-%S")
    logfile = f"/home/pi/log/log{name}.txt"

    uart = serial.Serial("/dev/serial0", baudrate=115200, timeout=10)
    gps = adafruit_gps.GPS(uart, debug=False)
    gps.send_command(b"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
    gps.send_command(b"PMTK220,1000")
    i = 0
    output_file = "Data_GPS.txt"
    while True:
            hora = datetime.datetime.now()
            gps.update()
            time.sleep(5)
            if not gps.has_fix:
                send_msg(chr_to_hex('Esperando satelite'))
               # with open(output_file,"a") as f:
#                    f.write("Esperando satelite" +" \n")
                print("Esperando satelite")
                continue
            try:
                hora = datetime.datetime.now()
                strin1 = "Latitud : {0:.6f} grados".format(gps.latitude)
                strin2 = "Longitud : {0:.6f} grados".format(gps.longitude)
#                strin3 = "Altitud: {0:.6f} grados".format(gps.altitude_m)
               # mensaje1 = f"[{hora}] {strin1} {strin2} {strin3}"
               # mensaje1 =  f"[{hora}"],{
               # mensaje2 = strin2 + strin3
                with open(output_file, "a") as f:
                    f.write(mensaje + "\n")
                if i%10==0:
                   send_msg(chr_to_hex(strin1))
                   send_msg(chr_to_hex(strin2))
                   send_msg(chr_to_hex(strin3))
                   print(mensaje1)
                i+=1

            except Exception:
                hora = datetime.datetime.now()
                send_msg(chr_to_hex(f"[{hora}] No se ha recogido informacion"))
                print(f"[{hora}] Fallo al leer informacion... intentando de nuevo")

            except KeyboardInterrupt:
                print("Cerrando programa")
                exit(0)

if __name__ == "__main__":
    main(100,1)
