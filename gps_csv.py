import adafruit_gps
import serial
import time
import csv

gps = None #Objeto gps
interval = 1 #intervalo de tiempo entre muestras GPS

def main(interval_time):

    uart = serial.Serial("/dev/serial0", baudrate = 9600, timeout = 10) #Objeto serial
    gps = adafruit_gps.GPS(uart, debug=False) #Objeto GPS
    gps.send_command(b"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0") #Comando de inicio
    gps.send_command(b"PMTK220,1000") #Comando de inicio
    with open('datos_GPS.csv', mode='a', newline='') as file: #Se trabaja sobre el archivo CSV del GPS
        while True:   #Ciclo infinito de muestras
            if not gps.update() or not gps.has_fix: #Si no hay actualizicacion o no hay satelite fijo
                print("Esperando")   
                time.sleep(1)
                continue
            latitud = gps.latitude    
            longitud = gps.longitude
            altitud = gps.altitude_m
            timestamp = gps.timestamp_utc 
            data_line = 'Latidud:', latitud,' Longitud:',longitud, 'altitud:',altitud, 'tiempo:', timestamp
            print(data_line)
            file.write(data_line)   #Se escribe los datos en el CSV
            time.sleep(interval_time)
        
if __name__ == "__main__":
    main(interval)






