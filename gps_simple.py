import adafruit_gps
import serial
import time

gps = None


def main(n_msr, interval_time):

    uart = serial.Serial("/dev/serial10", baudrate = 9600, timeout = 10)
    gps = adafruit_gps.GPS(uart, debug=False)
    gps.send_command(b"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
    gps.send_command(b"PMTK220,1000")
    while True:
        if not gps.update() or not gps.has_fix:
            time.sleep(0.1)
            continue
        latitud = gps.latitude()
        longitud = gps.longitude()
        print('Latidud:', latitud,' Longitud:',longitud)
        time.sleep(1)
        
if __name__ == "__main__":
    main(1,1)






