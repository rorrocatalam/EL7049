import smbus2
from time import sleep

#BMP_280 Address

bmp_addr= 0x76

#get acces to the 12c bus
i2c = smbus2.SMBus(1)

#setup the config register
i2c.write_byte_data(bmp_addr,0xf5,(5<<5))
i2c.write_byte_data(bmp_addr,0xf4,(5<<5)|(3<<0))

#sensor is set up and will do a measurment every sec
#temperatura
dig_T1 = i2c.read_word_data(bmp_addr,0x88)
dig_T2 = i2c.read_word_data(bmp_addr,0x8A)
dig_T3 = i2c.read_word_data(bmp_addr,0x8C)

#presion

dig_P1 = i2c.read_word_data(bmp_addr,0x8E)
dig_P2 = i2c.read_word_data(bmp_addr,0x90)
dig_P3 = i2c.read_word_data(bmp_addr,0x92)
dig_P4 = i2c.read_word_data(bmp_addr,0x94)
dig_P5 = i2c.read_word_data(bmp_addr,0x96)
dig_P6 = i2c.read_word_data(bmp_addr,0x98)
dig_P7 = i2c.read_word_data(bmp_addr,0x9A)
dig_P8 = i2c.read_word_data(bmp_addr,0x9C)
dig_P9 = i2c.read_word_data(bmp_addr,0x9E)

# se realizan arreglos a los short
if(dig_T2>32767):
    dig_T2-=65536
if(dig_T3>32767):
    dig_T3-=65536
if(dig_P2>32767):
    dig_P2-=65536
if(dig_P3>32767):
    dig_P3-=65536
if(dig_P4>32767):
    dig_P4-=65536
if(dig_P5>32767):
    dig_P5-=65536  
if(dig_P6>32767):
    dig_P6-=65536
if(dig_P7>32767):
    dig_P7-=65536
if(dig_P8>32767):
    dig_P8-=65536
if(dig_P9>32767):
    dig_P9-=65536    

#Read the raw temperature
while True:

    d1 =i2c.read_byte_data(bmp_addr,0xfa)
    d2 =i2c.read_byte_data(bmp_addr,0xfb)
    d3 =i2c.read_byte_data(bmp_addr,0xfc)

    adc_T = ((d1<<16)|(d2<<8) | d3)>>4

    p1 =i2c.read_byte_data(bmp_addr,0xf7)
    p2 =i2c.read_byte_data(bmp_addr,0xf8)
    p3 =i2c.read_byte_data(bmp_addr,0xf9)

    adc_P = ((p1<<16)|(p2<<8) | p3)>>4

    #calculate de temperature

    var1 = (((adc_T>>3) -(dig_T1<<1))* (dig_T2) )>>11
    var2 = ((((adc_T >> 4) - ((dig_T1)) * ((adc_T >> 4) - (dig_T1))) >> 12) * (dig_T3)) >> 14
    t_fine = var1 + var2
    T = (t_fine * 5 + 128) >> 8
    T =T/100

    # se calcula la presion
    var1 = (t_fine) - 128000
    var2 = var1 * var1 * dig_P6
    var2 = var2 + ((var1 * dig_P5) << 17)
    var2 = var2 + ((dig_P4) << 35)
    var1 = ((var1 * var1 * dig_P3) >> 8) + ((var1 * dig_P2) << 12)
    var1 = (((1 << 47) + var1) * (dig_P1)) >> 33

    if var1==0:
        print("error")
    

    p = 1048576 - adc_P
    p = (((p << 31) - var2) * 3125) / var1
    var1 = ((dig_P9) * (p >> 13) * (p >> 13)) >> 25
    var2 = ((dig_P8) * p) >> 19
    p = ((p + var1 + var2) >> 8) + ((dig_P7) << 4)
    p= p / 25600.0

    print(f"Temperatura :{T} C, Presion:{p} hPa")

    sleep(1)
