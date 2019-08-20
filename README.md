#XDK110_Client


##Introduction

XDK 110 inclusive of multiple micro electromechanical systems sensors, various parameters for condition monitoring or predictive maintenance get recorded.
This code is client side of collecting data via 8 sensors through UDP from XDK 110 .

*Sensor settings :*

>###### 1.Accelerometer:
   Type:collecting X , Y and Z axis values<br />
   Range: ±2 ~ ±16 g (programmable)<br />
   Rate: 2000 Hz<br />
   Unit: g <br />


>###### 2.Magnetometer:
   Type:detecting the strength of the earth’s (or any local) magnetic field through X , Y , Z and R axis values<br />
   Range: ±1300 μT (X,Y-Axis); ±2500μT (Z-Axis)<br />
   Rate: 300 Hz<br />
   Unit: mT<br />


>###### 3.Gyroscope:
   Type:collecting data via three axes of the gyroscope sensor<br />
   Range: ±125 º/s ~ ±2000 º/s (programmable)<br />
   Rate: 2000 Hz<br /> 
   Unit: deg/s <br />


>###### 4.Humidity: 
   Range: 10 ~ 90 %rH (non-condensing)[limited by XDK operating conditions]<br />
   Rate: 182 Hz<br /> 
   Unit: %rh<br />


>###### 5.Pressure:
   Range: 300 ~ 1100 hPa<br />
   Rate: 182 Hz<br /> 
   Unit: Pa<br />

>###### 6.Light:
   Range: 0.045 lux ~ 188,000 lux ; 22-bit<br />
   Rate: -<br />
   Unit: lux<br />


>###### 7.Noise:
   Range: average voltage peaks of 500-700 mV<br />
   Rate: - <br />
   Unit: mV<br />


>###### 8.Temperature:
   Range: -20 ºC ~ 60 °C [limited by XDK operating conditions]<br />
   Rate: 182 Hz<br /> 
   Unit: °C<br />


##Configuration

Install xdk workbench v.3.4<br /> 
Import code<br />
Connect sensor to computer via USB<br />
Clean>>Build>>Flash<br /> 


##Environment

-XDK Workbench 3.4.0



**Note:** *can not access to data from client side ,should run server separately.* 