# -*- coding: utf-8 -*-
"""Tutorial on using the InfluxDB client."""

import argparse
import time
import serial
import re
from influxdb import InfluxDBClient

import serial
import io

def main(host='localhost', port=8086):
    """Instantiate a connection to the InfluxDB."""
    user = 'admin'
    password = 'admin'
    dbname = 'rfHome'

    client = InfluxDBClient(host, port, user, password, dbname)

    ser = serial.Serial(port='COM6', baudrate=115200, stopbits=serial.STOPBITS_ONE,parity= serial.PARITY_NONE)

    value = 0.1
    while 1==1:
        s = ser.readline()  # read up to one hundred bytes
        s="".join(map(chr, s))
        if s.find("====")>0:
            s_tvco = ser.readline()
            s1 = "".join(map(chr, s_tvco))
            s2 = re.findall(r'[\d\.\d]+', s1)
            tvco = list(map(int,s2))
            #print("{0} \n",tvco[1])
            s_eCO2 = ser.readline()
            s1 = "".join(map(chr, s_eCO2))
            s2 = re.findall(r'[\d\.\d]+', s1)
            eCO2 = list(map(int, s2))
            #print("{0} \n",eCO2[2])
            s_temp = ser.readline()
            s1 = "".join(map(chr, s_temp))
            s2 = re.findall(r'[\d\.\d]+', s1)
            temp = list(map(float, s2))
            #print("{0} \n",temp[1])
            s_hum = ser.readline()
            s1 = "".join(map(chr, s_hum))
            s2 = re.findall(r'[\d\.\d]+', s1)
            hum = list(map(float, s2))
            #print("{0} \n", hum[1])
            s_watt = ser.readline()
            s1 = "".join(map(chr, s_watt))
            s2 = re.findall(r'[\d\.\d]+', s1)
            watt = list(map(int, s2))
            #print("{0} \n", watt[1])
            s_wattH = ser.readline()
            s1 = "".join(map(chr, s_wattH))
            s2 = re.findall(r'[\d\.\d]+', s1)
            wattH = list(map(int, s2))
            #print("{0} \n", wattH[1])

            json_body = [
                {
                    "measurement": "devSns",
                    "tags": {
                        "sensor": "dev1"
                    },
                    "fields": {
                        "TVCO": tvco[1],
                        "eCO2": eCO2[2],
                        "T": temp[1],
                        "H": hum[1],
                        "W": watt[1],
                        "Wh": wattH[1]
                    }
                }
            ]
            print(json_body)
            #print("Write points: {0}".format(json_body))
            client.write_points(json_body)
            time.sleep(1)



def parse_args():
    """Parse the args."""
    parser = argparse.ArgumentParser(
        description='example code to play with InfluxDB')
    parser.add_argument('--host', type=str, required=False,
                        default='localhost',
                        help='hostname of InfluxDB http API')
    parser.add_argument('--port', type=int, required=False, default=8086,
                        help='port of InfluxDB http API')
    return parser.parse_args()


if __name__ == '__main__':
    args = parse_args()
    main(host=args.host, port=args.port)