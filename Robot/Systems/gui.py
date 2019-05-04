#!/usr/bin/env python
# -*- coding: utf-8 -*-
import serial
from Vision.ImagePreProcess import *
from Vision.Image import *

#from Vision.Image import Operation
import os
import sys
from Vision import geo
from Vision.camDisplay import *
import numpy as np
import cv2
import serutil
from ser import *
from PyQt4 import QtGui
from PyQt4.QtCore import (QThread, Qt, pyqtSignal, pyqtSlot, QUrl)
from PyQt4.QtGui import (QPixmap, QImage, QApplication, QWidget, QLabel)
#class TextThread(QThread):

    #def __init__(self, parent=None):
        #QThread.__init__(self, parent=parent)
    #def run(self):
        #while True:

class Thread(QThread):
    changeText1 = pyqtSignal(str)
    changeText2 = pyqtSignal(str)
    changeText3 = pyqtSignal(str)
    changeText4 = pyqtSignal(str)
    changeText5 = pyqtSignal(str)
    changeText6 = pyqtSignal(str)
    changen = pyqtSignal(str)
    changet = pyqtSignal(str)
    changesq = pyqtSignal(str)
    changel = pyqtSignal(str)
    changec = pyqtSignal(str)
    changePixmap = pyqtSignal(QImage)
    changePixmap2 = pyqtSignal(QImage)
    changePixmap3 = pyqtSignal(QImage)


    def __init__(self, parent=None):
        QThread.__init__(self, parent=parent)
    def run(self):
        
        s = SerialUtil()
        wc = WhatsCrackin()
        proc = ImagePreProcess()
        op = Operation(0)
        op2 = Display(1)
        op3 = Operation(2)
        while True:
            ret, img = op.get()
            n,t,sq,l,c,img_2 = op2.get()
            ret, img_3 = op3.get()
            convertToQtFormat = QImage(img.data, img.shape[1],img.shape[0],QImage.Format_RGB888).rgbSwapped()
            convertToQtFormat_2 = QImage(img_2.data, img_2.shape[1],img_2.shape[0],QImage.Format_RGB888).rgbSwapped()
            convertToQtFormat_3 = QImage(img_3.data, img_3.shape[1],img_3.shape[0],QImage.Format_RGB888).rgbSwapped()
            p = convertToQtFormat.scaled(960,540,Qt.KeepAspectRatio)
            p_2 = convertToQtFormat_2.scaled(960,540,Qt.KeepAspectRatio)
            p_3 = convertToQtFormat_3.scaled(960,540,Qt.KeepAspectRatio)
            self.changen.emit(n)
            self.changet.emit(t)
            self.changesq.emit(sq)
            self.changel.emit(l)
            self.changec.emit(c)
            self.changePixmap.emit(p)
            self.changePixmap2.emit(p_2)
            self.changePixmap3.emit(p_3)
            msg = s.get()
            if msg is not None:        
                t_housing_in,t_housing_out,h_housing_in,leak_sensor,x,y = msg
                self.changeText1.emit('Temperature inside housing: '+str(t_housing_in))
                self.changeText2.emit('Temperature inside housing: '+str(t_housing_out))
                self.changeText3.emit('Temperature inside housing: '+str(h_housing_in))
                self.changeText4.emit('Leak sensor: '+str(leak_sensor))
                self.changeText5.emit('X: '+x)
                self.changeText6.emit('Y: '+y)

class App(QWidget):
    def __init__(self):
        super(App, self).__init__()
        self.title = "45C Robotics 2019"
        self.initUI()
        self.setStyleSheet(open('style.css').read())
    @pyqtSlot(QImage)
    def setImage(self, image):
        self.videoCom.setPixmap(QPixmap.fromImage(image))
    @pyqtSlot(QImage)
    def setImage2(self, image):
        self.videoCom2.setPixmap(QPixmap.fromImage(image))
    @pyqtSlot(QImage)
    def setImage3(self, image):
        self.videoCom3.setPixmap(QPixmap.fromImage(image))
    @pyqtSlot(str)
    def setText1(self, text):
        self.t_housing_in_label.setText(text)     
    @pyqtSlot(str)
    def setText2(self, text):
        self.t_housing_o_label.setText(text)     
    @pyqtSlot(str)
    def setText3(self, text):
        self.h_housing_in_label.setText(text)     
    @pyqtSlot(str)
    def setText4(self, text):
        self.leak_sensor_label.setText(text)     
    @pyqtSlot(str)
    def setText5(self, text):
        self.x_label.setText(text)     
    @pyqtSlot(str)
    def setText6(self, text):
        self.y_label.setText(text) 
    @pyqtSlot(str)
    def setNumShapes(self, text):
        self.n_label.setText(text)         
    @pyqtSlot(str)
    def setNumTriangles(self, text):
        self.t_label.setText(text)         
    @pyqtSlot(str)
    def setNumSquares(self, text):
        self.sq_label.setText(text)         
    @pyqtSlot(str)
    def setNumLines(self, text):
        self.l_label.setText(text)         
    @pyqtSlot(str)
    def setNumCircles(self, text):
        self.c_label.setText(text) 
    
    def initUI(self):
        self.ser = serial.Serial('/dev/ttyUSB0',57600,timeout=1)
        self.ser.isOpen()
        print("Initialized serial comms")
        # Convention: (y, x)
        self.setWindowTitle(self.title)
        self.resize(1920,1080)
        # Number of shapes
        self.n_label = QLabel(self)
        self.n_label.setText('--- Number of shapes ---')
        self.n_label.setAlignment(Qt.AlignRight)
        self.n_label.move(1710,525)         
        # T Species
        self.t_label = QLabel(self)
        self.t_label.setText('--- # of Triangles ---')
        self.t_label.setAlignment(Qt.AlignRight)
        self.t_label.move(1710,555)         
        # Sq Species
        self.sq_label = QLabel(self)
        self.sq_label.setText('--- # of Squares ---')
        self.sq_label.setAlignment(Qt.AlignRight)
        self.sq_label.move(1710,585)         
        # Lines Species
        self.l_label = QLabel(self)
        self.l_label.setText('--- # of Lines ---')
        self.l_label.setAlignment(Qt.AlignRight)
        self.l_label.move(1710,615)         
        # Circles Species
        self.c_label = QLabel(self)
        self.c_label.setText('--- # of Circles ---')
        self.c_label.setAlignment(Qt.AlignRight)
        self.c_label.move(1710,645)         
        # Title label
        self.title_label = QLabel(self)
        self.title_label.setText('--- Operator Data ---')
        self.title_label.setAlignment(Qt.AlignRight)
        self.title_label.move(40,495)        
        # Temperature Inside Housing
        self.t_housing_in_label = QLabel(self)
        self.t_housing_in_label.setText('Temperature inside housing:')
        self.t_housing_in_label.setAlignment(Qt.AlignRight)
        self.t_housing_in_label.move(40,525)
        # Temperature Outside Housing
        self.t_housing_o_label = QLabel(self)
        self.t_housing_o_label.setText('Temperature outside housing:')
        self.t_housing_o_label.setAlignment(Qt.AlignRight)
        self.t_housing_o_label.move(40,555)
        # Humidity inside housing
        self.h_housing_in_label = QLabel(self)
        self.h_housing_in_label.setText('Humidity inside housing:')
        self.h_housing_in_label.setAlignment(Qt.AlignRight)
        self.h_housing_in_label.move(40,585)
        # Leak Sensor
        self.leak_sensor_label = QLabel(self)
        self.leak_sensor_label.setText('Leak sensor:')
        self.leak_sensor_label.setAlignment(Qt.AlignRight)
        self.leak_sensor_label.move(40,615)
        # x
        self.x_label = QLabel(self)
        self.x_label.setText('X:')
        self.x_label.setAlignment(Qt.AlignRight)
        self.x_label.move(40,645)
        # y
        self.y_label = QLabel(self)
        self.y_label.setText('Y:')
        self.y_label.setAlignment(Qt.AlignRight)
        self.y_label.move(40,675)
        # Video component 1
        self.videoCom = QLabel(self)
        self.videoCom.move(150,0)
        self.videoCom.resize(925,500)
        # Video component 2
        self.videoCom2 = QLabel(self)
        self.videoCom2.move(1055,0)
        self.videoCom2.resize(855,500)
        # Video component 3
        self.videoCom3 = QLabel(self)
        self.videoCom3.move(580,540)
        self.videoCom3.resize(1200,540)
        th = Thread(self)
        th.changePixmap.connect(self.setImage)
        th.changePixmap2.connect(self.setImage2)
        th.changePixmap3.connect(self.setImage3)
        th.changePixmap3.connect(self.setImage3)
        th.changen.connect(self.setNumShapes)
        th.changet.connect(self.setNumTriangles)
        th.changesq.connect(self.setNumSquares)
        th.changel.connect(self.setNumLines)
        th.changec.connect(self.setNumCircles)
        th.changeText1.connect(self.setText1) 
        th.changeText2.connect(self.setText2) 
        th.changeText3.connect(self.setText3) 
        th.changeText4.connect(self.setText4) 
        th.changeText5.connect(self.setText5) 
        th.changeText6.connect(self.setText6) 
        th.start()
    def abort(self):
        self.close()
if __name__ == "__main__":
    os.system("fuser -k /dev/video0")
    os.system("fuser -k /dev/video1")
    #os.system("fuser -k /dev/video3")

    app = QApplication(sys.argv)
    run = App()
    run.show()
    sys.exit(app.exec_())
