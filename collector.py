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
import time as t
import os
from logger import Logger
import multiprocessing
from Plotter import Plotter

import sys

from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl

lasttime = t.time()

class Collector (threading.Thread):
    # this is in the main process

    def __init__(self, threadID, name, configs):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name

        #self.logger = logger

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

        self.last_time = t.time()

    def stop_logger(self):
        logger, queue, proc = self.logger

        queue.close()
        queue.join_thread()
        proc.join()

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
                try:
                    response = uart.readline().decode()
                    print("%s\n" % response)
                except Exception as ex:
                    print(ex)

    def unserialize(self, chunk):
        # check magic word
        # print(chunk[0])
        # if chunk[0] != 0x0b:
        #     return

        # check first few elements

        #file = open('data_'+ str(s)+'.dat' , "a")
        print(" ".join(str(v) for v in list(chunk)))
        #file.write(" ".join(str(v) for v in list(chunk)))
        # sync good
        framedata = self.bytesarray_to_floatarray(chunk)
        print(np.shape(framedata))
        framedata = np.reshape(framedata, (self.GESTURE_PKT_SIZE_BYTES >> 2, self.NUM_PKTS_TO_COLLECT))

    def read_data(self, uart):
        global lasttime

        filename = "2-" + self.name + str(".dat")
        file = open(filename, "a")

        while True:
            # this will take some time
            if uart.in_waiting > 0:
                bytes = uart.read(uart.in_waiting)
                file.write(" ".join(str(v) for v in list(bytes)))

                # send to data logger
                nowtime = t.time()


                print(self.threadID, " write ", len(bytes), " to the file, ", float(len(bytes) / (nowtime - lasttime)))
                lasttime = nowtime
                #self.logger[1].put(bytes)
                # send to data parser
                #Plotter.coordinates[self.name].extend(bytes)   # append to byte stream

    def run(self):
        uart_port = serial.Serial(self.uart_port, baudrate=self.uart_port_baudrate, timeout=self.uart_port_timeout)
        data_port = serial.Serial(self.data_port, baudrate=self.data_port_baudrate, timeout=self.data_port_timeout)
        cmds = self.read_configure("profile3.cfg")
        self.write_cmds(uart_port, cmds)
        self.read_data(data_port)

# load configurations
with open('ports.json') as f:
    info = json.load(f)
#p = Plotter()
pprint(info)
threads = {}
# spawn out the thread

#p.start(1000)    # main process is for rendering graph


try:
    q_list = {}

    for k, v in info.items():
        # create a logger running at another process
 #       logger = Logger("test", info[k]["id"])
 #       q_list[info[k]["id"]] = multiprocessing.Queue()   # data collection occurred in a separated thread
 #       proc = multiprocessing.Process(target=logger.run, args=(q_list[info[k]["id"]],))

   #     c = Collector(int(k), info[k]["id"], info[k], (logger, q_list[info[k]["id"]], proc))
        c = Collector(int(k), info[k]["id"], info[k])
        #Plotter.add_node(info[k]["id"])

        c.start() # spawning a separated thread for gathering data
        threads[k] = c

        # proc.start() # start the logger in a separated process

except Exception as ex:
    print(ex)


# if (sys.flags.interactive != 1) or not hasattr(QtCore, PYQT_VERSION):
#     QtGui.QApplication.instance().exec_()


# wait to join
for k, v in threads.items():
    threads[k].join()
    #threads[k].stop_logger()

print("exiting")

# handa sensing
# 2 separated radar
# ground true
# focus on one experiemtnt:
# noise cancellation
# Synchronization
