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
    with open('/home/pi/suchai-flight-software/apps/sonda/data/data_gps.csv', mode='w', newline='') as file: #Se trabaja sobre el archivo CSV del GPS
        
        idx_csv = 1
        while True:   #Ciclo infinito de muestras
            if not gps.update() or not gps.has_fix: #Si no hay actualizicacion o no hay satelite fijo
                #print("Esperando GPS...")   
                time.sleep(1)
                continue
            lat = gps.latitude    
            lon = gps.longitude
            alt = gps.altitude_m
            timestamp = gps.timestamp_utc
            year = timestamp.tm_year
            mon = timestamp.tm_mon
            day = timestamp.tm_nday
            hour = timestamp.tm_hour
            min = timestamp.tm_min
            sec = timestamp.tm_sec
            sec = sec[:2]
            date = f"{year};{mon};{day};{hour};{min};{sec}"

            # Latitud, Longitud, Altitud, Fecha
            data_line = f"{idx_csv};{lat};{lon};{alt};{date};\n"
            idx_csv +=1
            #'Latidud:', latitud,' Longitud:',longitud, 'altitud:',altitud, 'tiempo:', timestamp
            #print(data_line)
            file.write(data_line)   #Se escribe los datos en el CSV
            file.flush()
            time.sleep(interval_time)
        
if __name__ == "__main__":
    main(interval)