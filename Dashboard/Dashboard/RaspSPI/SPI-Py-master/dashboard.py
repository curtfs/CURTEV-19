
import RPi.GPIO as GPIO
from time import sleep
import datetime
#from firebase import firebase


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
    



    #update_firebase()
SPI_Rasp()
        
       








