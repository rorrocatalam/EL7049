import adafruit_gps
import serial
import time

gps = None


def main(n_msr, interval_time):

    uart = serial.Serial("/dev/serial0", baudrate = 9600, timeout = 10)
    gps = adafruit_gps.GPS(uart, debug=False)
    gps.send_command(b"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
    gps.send_command(b"PMTK220,1000")
    while True:
        if not gps.update() or not gps.has_fix:
            print("Esperando")
            time.sleep(1)
            continue
        latitud = gps.latitude
        longitud = gps.longitude
        altitud = gps.altitude_m
        print('Latidud:', latitud,' Longitud:',longitud, 'altitud:',altitud)
        time.sleep(5)
        
if __name__ == "__main__":
    main(1,1)






