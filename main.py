import time
import serial
import numpy as np
import matplotlib.pyplot as plt

NUM_PKTS_TO_COLLECT = 8;
GESTURE_PKT_SIZE_BYTES= 48;
FRAME_SIZE = GESTURE_PKT_SIZE_BYTES * NUM_PKTS_TO_COLLECT # in bytes

vel_resolution = 0.0380479;
range_resolution = 0.046875;

def read_configure(filename):
    cmd = open(filename, 'r')
    cmd = cmd.readlines()
    cmd = [e[:-1].encode('ascii') for e in cmd]
    return cmd

def write_cmds(uart, cmds):
    try:
        uart.open()
    except Exception as ex:
        print(str(ex))

    for cmd in cmds:
        print("sending: ", cmd)
        uart.write(cmd)
        uart.write('\n'.encode('ascii'))
        uart.flush()
        while ser.out_waiting > 0:
            pass
        while ser.in_waiting <= 0:
            pass
        for r in range(3):
            time.sleep(0.5)
            response = uart.readline().decode()
            print("%s\n" % response)

def quick_start(uart):
    write_cmds(uart, ['sensorStop'.encode('ascii'), 'sensorStart'.encode('ascii')])

# TODO big or small endianess?? - need confirm
def bytesarray_to_floatarray(bytes):
    floatarray = []
    bytesize = len(bytes)
    i = 0
    while i < bytesize:
        floatarray.append(bytes[i] | bytes[i + 1] << 8 | bytes[i + 2] << 16 | bytes[i + 3] << 24)
        i += 4
    return np.array(floatarray, dtype = np.float64)  # 96 numbers

# TODO:
def unserialize(chunk):
    # check magic word
    # print(chunk[0])
    # if chunk[0] != 0x0b:
    #     return

    # check first few elements
    print("first few bytes ", chunk[0], , " ", chunk[1], " ", chunk[2], " ", chunk[3])

    # sync good
    framedata = bytesarray_to_floatarray(chunk)
    print(np.shape(framedata))
    framedata = np.reshape(framedata, (GESTURE_PKT_SIZE_BYTES >> 2, NUM_PKTS_TO_COLLECT))

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

def read_data(uart):
    buf = bytearray() # mutable buf

    while True:
        if uart.in_waiting > 0:
            bytes = uart.read(uart.in_waiting)
            buf.extend(bytes)
            print("received bytes len: ", len(buf))

            if len(buf) >= FRAME_SIZE:
                # send to data parser
                chunk = buf[0: FRAME_SIZE]
                unserialize(chunk)
                del buf[0: FRAME_SIZE]

# 115200 is the cmd baud rate
ser = serial.Serial('/dev/ttyACM0', baudrate = 115200, timeout = 0)
data_port = serial.Serial('/dev/ttyACM1', baudrate = 921600, timeout = 0.3)

cmds = read_configure("config.cfg")
write_cmds(ser, cmds)
#quick_start(ser)
read_data(data_port)