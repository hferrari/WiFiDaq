#!/usr/bin/env python
import socket
import time
import pyqtgraph as pg
import numpy as np
import sys
from pyqtgraph.Qt import QtGui, QtCore

#Socket initialization
HOST = "10.42.0.77"
PORT = 80
socket.setdefaulttimeout(1)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

#Graph initialization
app = QtGui.QApplication([])
win = pg.GraphicsWindow(title="Data Acquisition")
win.resize(1366,768)
pg.setConfigOptions(antialias=True)

PRES_VECTOR_SIZE = 100
TEMP_VECTOR_SIZE = 100
ALT_VECTOR_SIZE = 100
POT_VECTOR_SIZE = 100

presGraph = win.addPlot(title="Pressao Atmosferica (hPa)")
presGraph.setRange(yRange=[940, 950])

tempGraph = win.addPlot(title="Temperatura Ambiente (C)")
tempGraph.setRange( yRange=[25, 35])

altGraph = win.addPlot(title="Altitude (m)")
altGraph.setRange(yRange=[550, 620])

win.nextRow()

potGraph = win.addPlot(title="Potenciometro (%)", colspan=3)
potGraph.setRange(yRange=[-5, 105])


curvePres = presGraph.plot(pen=(255,0,0))
curveTemp = tempGraph.plot(pen=(0,255,0))
curveAlt = altGraph.plot(pen=(0,0,255))
curvePot = potGraph.plot(pen=(255,0,255))

pres = np.empty(PRES_VECTOR_SIZE)
temp = np.empty(TEMP_VECTOR_SIZE)
alt = np.empty(ALT_VECTOR_SIZE)
pot = np.empty(POT_VECTOR_SIZE) 

presCounter = 0
tempCounter = 0
altCounter = 0
potCounter = 0

time.sleep(1)        

def recvData():
   return s.recv(128)

def requestData():
   s.send("GET\r\n")

def _main():
   
    global pres,presCounter
    global temp, tempCounter
    global alt, altCounter
    global pot, potCounter
    
    requestData()

    data = recvData().split(",")

    print "# "+ str(presCounter) + " -----> " + str(data)

    #Desloca os vetores uma posicao 
    pres[:-1] = pres[1:]
    temp[:-1] = temp[1:]
    alt[:-1] = alt[1:]
    pot[:-1] = pot[1:]

    #Coloca a amostra mais recente
    pres[-1] = data[2]
    temp[-1] = data[3]
    alt[-1] = data[4]
    pot[-1] = data[5]

    #Incrementa os contadores
    presCounter += 1
    tempCounter += 1
    altCounter += 1
    potCounter += 1

    #Atualiza a curva e desloca o eixo
    curvePres.setData(pres)
    curvePres.setPos(presCounter, 0)
	
    curveTemp.setData(temp)
    curveTemp.setPos(tempCounter, 0)

    curveAlt.setData(alt)
    curveAlt.setPos(altCounter, 0)

    curvePot.setData(pot)
    curvePot.setPos(potCounter, 0)


timer = QtCore.QTimer()
timer.timeout.connect(_main)
timer.start(1000)

if __name__ == "__main__":
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
         QtGui.QApplication.instance().exec_()

