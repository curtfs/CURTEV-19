
import RPi.GPIO as GPIO
from time import sleep
import datetime
#from firebase import firebase
# always seem to need this
import sys
 
# This gets the Qt stuff
import PyQt5
from PyQt5.QtWidgets import *
 
# This is our window from QtCreator
import mainwindow_auto

#import urllib2, urllib, httplib
import json
import os 
from functools import partial

import spi

def SPI_Rasp():
    # Open file descriptor for 
    # spi device 0 using the CE0 pin for chip select
    device_0 =  spi.openSPI(device="/dev/spidev0.0",
                            mode=0,
                            speed=1000000)

 

    # Transact data
    data_out = (0x05,)

    # This is not necessary, not just demonstrate loop-back
    data_in = (0x00,)
    data_in = spi.transfer(device_0, data_out)
    #print(data_in)
    while(1):
        data_in = spi.transfer(device_0, data_out)
        if data_in == (0x20,):
            print("Received from device 0:")
            print(data_in)

     
    # Close file descriptors
    #spi.closeSPI(device_0)
  


def update_firebase():
    
    firebase = firebase.FirebaseApplication('https://curt-dashboard.firebaseio.com/', None)
    Speedometer=20
    Battery=50
    Temprature=10
    Suspension=30
    Yaw=30
    Pitch=45
    Roll=90
    longAcc=20
    latAcc=10
    distance=100
    Mode="Racing"
    Faultmsg="No Fault ystaaaa"
    throttle=10
       

    
    #data = {"Speedometer": Speedometer, "Fault Message": Faultmsg, "Throttle Position": throttle "Battery Percentage": Battery, "Temprature": Temprature, "Suspension Travel ": Suspension, "Yaw angle": Yaw, "Pitch angle": Pitch, "Roll angle": Roll, "Longitude Acceleration": longAcc, "lateral": latAcc, "Covered Distance": distance, "Mode": Mode}
    #firebase.post('/dashboard', data)
    

# create class for our Raspberry Pi GUI
class MainWindow(QMainWindow, mainwindow_auto.Ui_MainWindow):
 # access variables inside of the UI's file
 def setspeed(self):
 print("Speed is:"+self.speedigtal.display(10))
 
 def __init__(self):
 super(self.__class__, self).__init__()
 self.setupUi(self) # gets defined in the UI file
 
 
 
# I feel better having one of these
def main():
 # a new app instance
 app = QApplication(sys.argv)
 form = MainWindow()
 form.show()
 # without this, the script exits immediately.
 sys.exit(app.exec_())
 
# python bit to figure how who started This
if __name__ == "__main__":
 main()

#update_firebase()
#SPI_Rasp()
        
       








