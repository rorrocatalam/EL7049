# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

# pylint: disable=wrong-import-position
# CircuitPython / Blinka
import board
import serial
import time
import csv
from adafruit_rockblock import RockBlock
import csv
uart = serial.Serial("/dev/ttyUSB0",19200)

def line_csv(file_csv):
    with open(file_csv, 'r') as archivo:
        # Lee el archivo CSV utilizando el lector de CSV de Python
        read_csv = csv.reader(archivo)
        # Itera sobre las líneas del archivo CSV
        for line in read_csv:
            last_line = line
        return last_line

def main(file_csv):
    rb = RockBlock(uart)
    last_line = line_csv(file_csv)
    print(f"the last line is: {last_line}")
    rb.data_out = str(last_line).encode()
    retry = 0
    print("Hablando con le moooooon")
    status = rb.satellite_transfer()
    while status[0] > 8 and retry < 10:
        time.sleep(1)
        status = rb.satellite_transfer()
        print(f"retrying! {retry}")
        retry+=1
    print("Done!")

