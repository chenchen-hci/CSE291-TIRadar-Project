import time
import serial
import numpy as np
import matplotlib.pyplot as plt
import _thread
import time
import threading
import json
from pprint import pprint
import struct

class Collector (threading.Thread):

    def __init__(self, threadID, name, configs):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name

        self.NUM_PKTS_TO_COLLECT = 8;
        self.GESTURE_PKT_SIZE_BYTES= 48;
        self.FRAME_SIZE = self.GESTURE_PKT_SIZE_BYTES * self.NUM_PKTS_TO_COLLECT # in bytes

        self.vel_resolution = 0.0380479;
        self.range_resolution = 0.046875;

        self.data_port = configs['data']['port']
        self.data_port_baudrate = configs['data']['baudrate']
        self.data_port_timeout = configs['data']['timeout']

        self.uart_port = configs['uart']['port']
        self.uart_port_baudrate = configs['uart']['baudrate']
        self.uart_port_timeout = configs['uart']['timeout']

    def read_configure(self, filename):
        cmd = open(filename, 'r')
        cmd = cmd.readlines()
        cmd = [e[:-1].encode('ascii') for e in cmd]
        return cmd

    def write_cmds(self, uart, cmds):
        try:
            uart.open()
        except Exception as ex:
            print(str(ex))

        for cmd in cmds:
            print("sending: ", cmd)
            uart.write(cmd)
            uart.write('\n'.encode('ascii'))
            uart.flush()
            while uart.out_waiting > 0:
                pass
            while uart.in_waiting <= 0:
                pass
            for r in range(3):
                time.sleep(0.5)
                response = uart.readline().decode()
                print("%s\n" % response)

    def bytesarray_to_floatarray(self, bytes):
        floatarray = []
        bytesize = len(bytes)
        i = 0
        while i < bytesize:
            var = bytes[i] | bytes[i + 1] << 8 | bytes[i + 2] << 16 | bytes[i + 3] << 24
            floatarray.append(struct.unpack('<f', struct.pack('I', var))[0])
            i += 4
        return np.array(floatarray, dtype = np.float64)  # 96 numbers

    def unserialize(self, chunk):
        # check magic word
        # print(chunk[0])
        # if chunk[0] != 0x0b:
        #     return

        # check first few elements
        print("first few bytes ", chunk[0], " ", chunk[1], " ", chunk[2], " ", chunk[3])

        # sync good
        framedata = self.bytesarray_to_floatarray(chunk)
        print(np.shape(framedata))
        framedata = np.reshape(framedata, (self.GESTURE_PKT_SIZE_BYTES >> 2, self.NUM_PKTS_TO_COLLECT))

        numdetections = framedata[1, :]
        dopplerave = framedata[2, :]
        rangeave = framedata[3, :]
        magsum = framedata[4, :]
        angleval = framedata[5, :]
        rangeidx = framedata[6, :]
        pkt_counter = framedata[8, :]
        range_disp = framedata[9, :]
        doppler_disp = framedata[10, :]
        prediction_result = framedata[11, :]
        # plt.plot(dopplerave * vel_resolution)
        # plt.show()
        # plt.pause(0.0001)

        print(pkt_counter)

    def read_data(self, uart):
        buf = bytearray() # mutable buf

        while True:
            if uart.in_waiting > 0:
                bytes = uart.read(uart.in_waiting)
                buf.extend(bytes)
                print(str(self.threadID) + " received bytes len: ", len(buf))

                if len(buf) >= self.FRAME_SIZE:
                    # send to data parser
                    chunk = buf[0: self.FRAME_SIZE]
                    self.unserialize(chunk)
                    del buf[0: self.FRAME_SIZE]

    def run(self):
        uart_port = serial.Serial(self.uart_port, baudrate=self.uart_port_baudrate, timeout=self.uart_port_timeout)
        data_port = serial.Serial(self.data_port, baudrate=self.data_port_baudrate, timeout=self.data_port_timeout)
        cmds = self.read_configure("config.cfg")
        self.write_cmds(uart_port, cmds)
        self.read_data(data_port)

# load configurations
with open('ports.json') as f:
    info = json.load(f)

pprint(info)
threads = {}
# spawn out the thread
try:
    for k, v in info.items():
        c = Collector(int(k), "collector", info[k])
        c.start()
        threads[k] = c
except Exception as ex:
    print(ex)

# wait to join
for k, v in threads.items():
    threads[k].join()
print("exiting")