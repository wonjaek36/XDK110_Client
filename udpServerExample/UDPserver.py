import socket
import sys
import json

from datetime import datetime
from pandas import DataFrame
import numpy as np
from matplotlib import pyplot as plt

xdk_sensors = {
    'Accel_X': [], 'Accel_Y': [], 'Accel_Z': [],
    'Mag_X': [], 'Mag_Y': [], 'Mag_Z': [], 'Mag_R': [],
    'Gyro_X': [], 'Gyro_Y': [], 'Gyro_Z': [],
    'RH': [],
    'Pressure': [],
    'Light': [],
    'Noise': [],
    'Temp': [],
    'Time': []
}
xdk_sensors_name = list(xdk_sensors.keys())

msgFromServer = "Hello UDP Client"
bytesToSend = str.encode(msgFromServer)

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the port
server_address = ('223.194.70.110', 3500)  # destination should be odroid
# print('Starting up on server {} port {}\n'.format(*server_address))
sock.bind(server_address)


while True:
    i = 0  # index i of keys in xdk_sensors dictionary above
    now = datetime.now()
    current_time = (str(now.hour)+':'+str(now.minute)+':'+str(now.second))  # timestamp for x axis in the graph
    xdk_sensors['Time'].append(current_time)

    data, address = sock.recvfrom(2000)
    json_data = json.loads(data)
    keys = list(json_data.keys())
    for key in keys:
        result_data = float(json_data[key])
        if key.find("Accel") == 0:
            result_data = result_data/float(1000)  # (unit) conversion to [G]
            print(key, ': ', result_data, '[G]')
        elif key.find("Mag") == 0:
            result_data = result_data/float(1000)  # conversion to [Tesla]
            print(key, ': ', result_data, '[Tesla]')
        elif key.find("Gyro") == 0:
            result_data = result_data/float(1000)  # conversion to [Deg/s]
            print(key, ': ', result_data, '[Deg/s]')
        elif key.find("Pressure") == 0:
            result_data = result_data / float(101325)  # conversion to [ATM]
            print(key, ': ', result_data, '[ATM]')
        elif key.find("Temp") == 0:
            result_data = result_data / float(1000)  # conversion to [Celsius]
            print(key, ': ', result_data, '[Celsius]')
        elif key.find("Light") == 0:
            result_data = result_data / float(1000)  # conversion to [lx]
            print(key, ': ', result_data, '[lx]')
        elif key.find("Noise") == 0:
            print(key, ': ', result_data, '[RMS Voltage]')
        xdk_sensors[xdk_sensors_name[i]].append(result_data)
        i = i+1

    # Show data into graph after 30 minutes
    # Due to memory issue, collect enough data first, and then make it into graph ...
    df = DataFrame(xdk_sensors, columns=['Time', 'Accel_X', 'Accel_Y', 'Accel_Z', 'Pressure'])
    df.plot('Time', ['Accel_X', 'Accel_Y', 'Accel_Z', 'Pressure'], kind='line')
    plt.show()
    print('\n')
