#!/usr/bin/python
import sys
import serial
import time
import datetime

now = datetime.datetime.now()
print "Almacenando datos en /home/pi/Desktop/Datos/"+now.strftime("%Y-%m-%d_%H:%M:%S")+".csv"
dataFile = open("/home/pi/Desktop/Datos/"+now.strftime("%Y-%m-%d_%H:%M:%S")+".csv", "w")
dataFile.write("Fecha,Metano,Temperatura(C),Humedad(%),Presion(mbar)\n")
arduino = serial.Serial('/dev/ttyACM0', 9600)
txt = ''

try:
    print "Configurando dispositivo..."
    time.sleep(5)
    while (True):
        now = datetime.datetime.now()
        txt = now.strftime("%Y-%m-%d %H:%M:%S")+',';
        arduino.write("M")    
        time.sleep(1)
        while arduino.inWaiting() > 0:
            txt += arduino.read(1)
        dataFile.write(txt+"\n")
        values = txt.split(',')
        sys.stdout.write("Fecha\t\t\t\tMetano\t\tTemperatura(C)\tHumedad(%)\tPresion(mbar)\n")
        for data in values:
            sys.stdout.write(data+'\t\t')
        print ''
        txt = ''
        time.sleep(1)
except KeyboardInterrupt:
    dataFile.close();
    arduino.close()