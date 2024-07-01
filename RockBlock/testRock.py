# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

# pylint: disable=wrong-import-position
# CircuitPython / Blinka
import board
import serial
import time
import csv
uart = serial.Serial("/dev/ttyUSB0",19200)
#uart = busio.UART(board.TX, board.RX, baudrate=19200)
#uart.baudrate = 19200

# via USB cable
# import serial
# uart = serial.Serial("/dev/ttyUSB0", 19200)

import csv

def line_csv(file_csv):
    with open(file_csv, 'r') as archivo:
        # Lee el archivo CSV utilizando el lector de CSV de Python
        read_csv = csv.reader(archivo)
        # Itera sobre las líneas del archivo CSV
        for line in read_csv:
            last_line = line
        return last_line

file_csv = 'software/mensaje.csv'
last_line = line_csv(file_csv)
print(last_line)

from adafruit_rockblock import RockBlock

print("initailizing rockblock!")
rb = RockBlock(uart)
print("Initalized")

print(rb.model)
print("Sending Hello World")
rb.data_out = "".join(last_line).encode() # "Hello World"

retry = 0
print("Talking to the mooooooon")
status = rb.satellite_transfer()
print(status)
while status[0] > 8:
    time.sleep(10)
    status = rb.satellite_transfer()
    print(f"retrying retry: {retry} \t status: {status}")
    retry +=1
print("Done!")
