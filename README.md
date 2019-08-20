#XDK110_Client


##Introduction

XDK 110 inclusive of multiple micro electromechanical systems sensors, various parameters for condition monitoring or predictive maintenance get recorded.
This code is client side of collecting data via 8 sensors through UDP from XDK 110 .

###*Sensor settings :*

####1.Accelerometer:
   #Type:collecting X , Y and Z axis values
   #Range: ±2 ~ ±16 g (programmable)
   #Rate: 2000 Hz
   #Unit: g 


####2.Magnetometer:
   #Type:detecting the strength of the earth’s (or any local) magnetic field through X , Y , Z and R axis values
   #Range: ±1300 μT (X,Y-Axis); ±2500μT (Z-Axis)
   #Rate: 300 Hz
   #Unit: mT


####3.Gyroscope:
   #Type:collecting data via three axes of the gyroscope sensor 
   #Range: ±125 º/s … ±2000 º/s (programmable)
   #Rate: 2000 Hz 
   #Unit: deg/s 


####4.Humidity: 
   #Range: 10…90 %rH (non-condensing)[limited by XDK operating conditions]
   #Rate: 182 Hz 
   #Unit: %rh


####5.Pressure:
   #Range: 300…1100 hPa
   #Rate: 182 Hz 
   #Unit: Pa

####6.Light:
   #Range: 0.045 lux … 188,000 lux ; 22-bit
   #Rate: -
   #Unit: lux


####7.Noise:
   #Range: average voltage peaks of 500-700 mV
   #Rate: - 
   #Unit: mV


####8.Temperature:
   #Range: -20 ºC ... 60 °C [limited by XDK operating conditions]
   #Rate: 182 Hz 
   #Unit: °C


##Configuration

Install xdk workbench v.3.4 
Import code
Connect sensor to computer via USB
Clean>>Build>>Flash 


##Environment

-XDK Workbench 3.4.0



**Note:** *can not access to data from client side ,should run server separately.* 