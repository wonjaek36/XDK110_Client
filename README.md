#XDK110_Client


##Introduction

XDK 110 inclusives multiple micro electromechanical systems sensors, various parameters for condition monitoring or predictive maintenance get recorded .Furthermore ,drivers for all system components included.It posses PC and MAC based development tools for Windows, LINUX and MacOS.

This code is client side of collecting data via 8 sensors through UDP from XDK 110 .

#### Operation Conditions :

* Indoor use
* Operating temperature range (-20 ºC ~ 60 °C, (0 °C ~ 45 °C for battery charging))
* Storage temperature range (-20 ºC ~ 60 °C)
* Humidity range 10~90 %rH (non-condensing)
* Supply Voltage 5 V DC
* IP Rating : IP 30 (IEC 60529) [IP 30 : First Digit is Solids Protection and Second Digit is Liquids Protection]
  `The first digit indicates the level of protection that the enclosure provides against access to hazardous parts (e.g., electrical conductors, moving parts) and the ingress of solid foreign objects.The second digit indicates protection of the equipment inside the enclosure against harmful ingress of water.`
 

#### Sensor APIs :

>###### 1.Accelerometer:
   Type:collecting X , Y and Z axis values and shows the rate of change of the speed and velocity of an object<br />
   Range: -2 ~ +2 g (programmable : mutable range )<br />
   Rate: 2000 Hz<br />
   Unit: g (G-forced = g )<br />


>###### 2.Magnetometer:
   Type:detecting the strength of the earth’s (or any local) magnetic field through X , Y , Z and R axis values<br />
   Range: ±1300 μT (X,Y-Axis); ±2500μT (Z-Axis)<br />
   Rate: 300 Hz<br />
   Unit: mT (micro Tesla)<br />


>###### 3.Gyroscope:
   Type:collecting data via three axes of the gyroscope sensor<br />
   Range: ±125 º/s ~ ±2000 º/s (programmable  : mutable range)<br />
   Rate: 2000 Hz<br /> 
   Unit: deg/s <br />


>###### 4.Humidity: 
   Range: 10 ~ 90 %rH (non-condensing)[limited by XDK operating conditions]<br />
   Rate: 182 Hz<br /> 
   Unit: %rh (percentage relative humidity)<br />


>###### 5.Pressure:
   Range: 300 ~ 1100 hPa<br />
   Rate: 182 Hz<br /> 
   Unit: Pa (Pascal)<br />

>###### 6.Light:
   Range: 0.045 lux ~ 188,000 lux ; 22-bit<br />
   Rate: -<br />
   Unit: lux (Illuminance is a measurement of the light intensity at any point. It is equal to 1 Lumen per square meter.)<br />


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