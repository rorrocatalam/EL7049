import serial
import time
import codecs
import threading

serial_port = ""
baud_rate = 115200

# Radio set up
freq = 915
mod = "SF9"
band_width = 125
tx_pr = 8
rx_pr = 8
power = 22

# RF configuration string
rf_conf_str = "AT+TEST=RFCFG,{},{},{},{},{},{},OFF,OFF,OFF\n".format(freq, mod, band_width, tx_pr, rx_pr, power)

# Serial Object
ser = None  
send = False
usr = ""

def init():
    global usr, ser
    usr = "Estacion Terrena"
    serial_port = "/dev/ttyUSB0"
    ser = serial.Serial(serial_port, baud_rate)
    initialize_radio()
    print("Radio Initialized")
    print('Your username is: {}'.format(usr))
    print('Begining LoRa Radio Chat ...')

def initialize_radio():  # Test PASSED
    ser.write("AT+MODE=TEST\n".encode())
    time.sleep(0.5)
    try:
        print(ser.readline().decode())
    except UnicodeDecodeError:
        print("Error decoding message from radio initialization")

    ser.write(rf_conf_str.encode())
    time.sleep(0.5)
    try:
        print(ser.readline().decode())
    except UnicodeDecodeError:
        print("Error decoding message from radio configuration")

#def send_msg(message):
#    ser.write("AT+TEST=TXLRPKT,\"{}\"\n".format(message).encode())
#    time.sleep(0.5)

almacenamiento = "datos_recibidos.csv"
def receive_msg():
    ser.write("AT+TEST=RXLRPKT".encode())
    while True:
        while not send:
            if ser.inWaiting():
                try:
                    rx_msg = ser.readline().decode().strip()
                    if '+TEST: LEN' in rx_msg:
                        parts = rx_msg.split(',')
                        for part in parts:
                            if 'SNR' in part:
                                snr_value = part.split(':')[-1].strip()
                                print(f"SNR: {snr_value}")
                            if "RSSI" in part:
                                dbm_value = part.split(':')[-1].strip()
                                print(f"db: {dbm_value}")
                    if '+TEST: RX ' in rx_msg:
                        msg_data = rx_msg.split('\"')[-2]
                        data = hex_to_chr(msg_data)
                        with open("datos_recibidos.csv","a") as datos:
                        	datos.write(data + "\n")
                        	datos.flush()
                        	datos.close()
                        print(data + f"\n{usr}: ")
                        
                except UnicodeDecodeError:
                    print("Error decoding received message")
                except IndexError:
                    print("Error processing received message")

def chr_to_hex(string):
    return codecs.encode(string.encode(), 'hex').decode()

def hex_to_chr(string):
    try:
        return codecs.decode(string, 'hex').decode()
    except (ValueError, UnicodeDecodeError):
        return "Error decoding hex string"

listeting = threading.Thread(target=receive_msg, daemon=True)

if __name__ == "__main__":
    init()
    listeting.start()
    while True:
       # msg = input(f"{usr}: ")
       # msg = f"{usr} --> {msg}"
       # send = True
       # send_msg(chr_to_hex(msg))
       # ser.write("AT+TEST=RXLRPKT".encode())
       # time.sleep(0.5)
        send = False

