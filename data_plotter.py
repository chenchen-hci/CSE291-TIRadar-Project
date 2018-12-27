import multiprocessing

import numpy as np
import struct
import plot_cloud

f = open("log_6ft_node_distance_2ft_people_dis_6ft_node_2.dat", 'r')
data = f.readline()
data = data.split(" ")
data = [int(n) for n in data]

start_idx = 0
end_idx = -1

def validata_point(point):
    if point[0] < 10 and point[0] > -10 \
        and point[1] < 10 and point[1] > -10 \
        and point[2] < 10 and point[2] > -10:
        return True
    else:
        return False

def bytesarray_to_int(bytes, start_idx):
    return bytes[start_idx] | bytes[start_idx + 1] << 8 | bytes[start_idx + 2] << 16 | bytes[start_idx + 3] << 24


def bytesarray_to_floatarray(bytes):
    floatarray = []
    bytesize = len(bytes)
    i = 0
    while i < bytesize:
        var = bytes[i] | bytes[i + 1] << 8 | bytes[i + 2] << 16 | bytes[i + 3] << 24
        floatarray.append(struct.unpack('<f', struct.pack('I', var))[0])
        i += 4
    return np.array(floatarray, dtype=np.float64)  # 96 numbers


def bytesarray_to_int16(bytes, start_idx):
    sign = bytes[start_idx + 1] & 0x80
    var = bytes[start_idx] | bytes[start_idx + 1] << 8
    val =  struct.unpack('<i', struct.pack('I', var))[0]
    return (val - 65536) * (sign != 0) + (val) * (sign == 0)

result = []
data_len = len(data)
i = 0
while i < data_len:
    try:
        # in the newer version, the next 4 bytes are the timestamp
        if data[i] == 2 and \
                data[i + 1] == 1 and \
                data[i + 2] == 4 and \
                data[i + 3] == 3 and \
                data[i + 4] == 6 and \
                data[i + 5] == 5 and \
                data[i + 6] == 8 and \
                data[i + 7] == 7:
            tot = bytesarray_to_int(data, i + 12)
            frame_number = bytesarray_to_int(data, i + 20)
            n_cpu = bytesarray_to_int(data, i + 24)
            n_detect = bytesarray_to_int(data, i + 28)

            # analyze detected objects
            n_tlv = bytesarray_to_int(data, i + 32)
            for j in range(n_tlv):
                tlv_tag = bytesarray_to_int(data, i + 36)
                tlv_length = bytesarray_to_int(data, i + 36 + 4)

                if tlv_tag == 1:
                    n_object = data[i + 36 + 8] | data[i + 36 + 9] << 8
                    n_format = data[i + 36 + 10] | data[i + 36 + 11] << 8
                    nor = 2 ** n_format
                    # i + 36 + 12 (48)
                    base = 0
                    for k in range(n_object):
                        # floatarray.append(struct.unpack('<f', struct.pack('I', var))[0])
                        x = float(bytesarray_to_int16(data, i + 48 + 6 + base) / nor)
                        y = float(bytesarray_to_int16(data, i + 48 + 8 + base) / nor)
                        z = float(bytesarray_to_int16(data, i + 48 + 10 + base) / nor)

                        result.append([x, y, z])
                        print("get coordinate: ", [x, y, z])
                        base = base + 12
                else:
                    continue
            i = i + tot
        else:
            i = i + 1
    except Exception as ex:
        i = i + 1

result = np.array(result, dtype = np.float64)



#plot_cloud.plot_plot_cloud(result[start_idx:end_idx,0], result[start_idx:end_idx, 1], result[start_idx:end_idx, 2])

plot_cloud.plot_plot_cloud(result[:,0], result[:, 1], result[:, 2])