import multiprocessing

from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import numpy as np
import struct
from scipy.stats import multivariate_normal
from sklearn.cluster import DBSCAN
from request import post
import _thread
import time as t

class Plotter():
    coordinates = {}  # key: node name value: [x, y, z]
    def __init__(self):
        self.app = QtGui.QApplication([])
        self.w = gl.GLViewWidget()

        self.g = gl.GLGridItem()
        self.w.addItem(self.g)

        self.a = gl.GLAxisItem()
        self.w.addItem(self.a)

        # generate position
        pos = np.random.randint(0, 2 ** 16, size=(2 ** 16, 3))
        pos[:, 2] = np.abs(pos[:, 2])

        self.sp2 = gl.GLScatterPlotItem(pos=pos)
        self.w.addItem(self.sp2)

    def plotter_tmr (self, timeout = 50):
        '''
        func = update function
        :param func:
        :param timeout:
        :return:
        '''
        self.w.show()

        self.t = QtCore.QTimer()
        self.t.timeout.connect(self.update_plot)
        self.t.start(timeout)

    def update_plot(self):
        n3_cluster = 0
        n3_noise = 0
        n3_err = 0
        n1_cluster = 0
        n1_noise = 0
        n1_err = 0
        n2_cluster = 0
        n2_noise = 0
        n2_err = 0

        return
        try:
            data = Plotter.coordinates["node_1"]   # data.append for adding coordinates
            result1 = []
            # unserialize the data
            # find the magic word
            data_len = len(data)
            i = 0
            while i < data_len:
                if data[i] == 2 and \
                        data[i + 1] == 1 and \
                        data[i + 2] == 4 and \
                        data[i + 3] == 3 and \
                        data[i + 4] == 6 and \
                        data[i + 5] == 5 and \
                        data[i + 6] == 8 and \
                        data[i + 7] == 7:
                    tot = self.bytesarray_to_int(data, i + 12)
                    frame_number = self.bytesarray_to_int(data, i + 20)
                    n_cpu = self.bytesarray_to_int(data, i + 24)
                    n_detect = self.bytesarray_to_int(data, i + 28)

                    # analyze detected objects
                    n_tlv = self.bytesarray_to_int(data, i + 32)
                    for j in range(n_tlv):
                        tlv_tag = self.bytesarray_to_int(data, i + 36)
                        tlv_length = self.bytesarray_to_int(data, i + 36 + 4)

                        if tlv_tag == 1:
                            n_object = data[i + 36 + 8] | data[i + 36 + 9] << 8
                            n_format = data[i + 36 + 10] | data[i + 36 + 11] << 8
                            nor = 2 ** n_format
                            # i + 36 + 12 (48)
                            base = 0
                            for k in range(n_object):
                                x = float(self.bytesarray_to_int16(data, i + 48 + 6 + base) / nor) - 0.3048
                                y = float(self.bytesarray_to_int16(data, i + 48 + 8 + base) / nor)
                                z = float(self.bytesarray_to_int16(data, i + 48 + 10 + base) / nor)
                                result1.append([x, y, z])

                                base = base + 12
                        else:
                            continue
                    i = i + tot
                else:
                    i = i + 1

            if len(result1) > 0:
                result1 = np.array(result1)
                self.sp2.setData(pos = result1)
                Plotter.coordinates["node_1"]  = []
                # clustering the data from node 1:
                n1_cluster, n1_noise, n1_err = self.clustering(result1)
                print("from node 1: ", n1_cluster)

            data = Plotter.coordinates["node_2"]   # data.append for adding coordinates
            result2 = []
            # unserialize the data
            # find the magic word
            data_len = len(data)
            i = 0
            while i < data_len:
                if data[i] == 2 and \
                        data[i + 1] == 1 and \
                        data[i + 2] == 4 and \
                        data[i + 3] == 3 and \
                        data[i + 4] == 6 and \
                        data[i + 5] == 5 and \
                        data[i + 6] == 8 and \
                        data[i + 7] == 7:
                    tot = self.bytesarray_to_int(data, i + 12)
                    frame_number = self.bytesarray_to_int(data, i + 20)
                    n_cpu = self.bytesarray_to_int(data, i + 24)
                    n_detect = self.bytesarray_to_int(data, i + 28)

                    # analyze detected objects
                    n_tlv = self.bytesarray_to_int(data, i + 32)
                    for j in range(n_tlv):
                        tlv_tag = self.bytesarray_to_int(data, i + 36)
                        tlv_length = self.bytesarray_to_int(data, i + 36 + 4)

                        if tlv_tag == 1:
                            n_object = data[i + 36 + 8] | data[i + 36 + 9] << 8
                            n_format = data[i + 36 + 10] | data[i + 36 + 11] << 8
                            nor = 2 ** n_format
                            # i + 36 + 12 (48)
                            base = 0
                            for k in range(n_object):
                                x = float(self.bytesarray_to_int16(data, i + 48 + 6 + base) / nor) + 0.3048
                                y = float(self.bytesarray_to_int16(data, i + 48 + 8 + base) / nor)
                                z = float(self.bytesarray_to_int16(data, i + 48 + 10 + base) / nor)
                                result2.append([x, y, z])
                                base = base + 12
                        else:
                            continue
                    i = i + tot
                else:
                    i = i + 1

            if len(result2) > 0:
                result2 = np.array(result2)
                self.sp2.setData(pos = result2, color = [0.8, 0.6, 0.7, 0.5])
                Plotter.coordinates["node_2"]  = []
                n2_cluster, n2_noise, n2_err = self.clustering(result2)
                print("from node 2, ", n2_cluster)

            print("eeee ", np.shape(result1))


            if np.shape(result1)[0] == 0 and np.shape(result2)[0] > 0:
                n3_cluster, n3_noise, n3_err = self.clustering(result1)
            elif np.shape(result1)[0] > 0 and np.shape(result2)[0] == 0:
                n3_cluster, n3_noise, n3_err = self.clustering(result2)
            elif np.shape(result1)[0] > 0 and np.shape(result2)[0] > 0:
                n3_cluster, n3_noise, n3_err = self.clustering(np.concatenate((result1, result2), axis = 0))

            # post the data
            if True:
                try:
                    _thread.start_new_thread(post, (n1_cluster, n1_noise, n1_err, \
                                                            n2_cluster, n2_noise, n2_err,  \
                                                            n3_cluster, n3_noise, n3_err, \
                                                             t.time()))
                except Exception as ex:
                    print(ex)
                    print("unable to spawn a new thread!")

        except Exception as ex:
            print(ex)
            print("err")

    def validata_point(self, point):
        if point[0] < 1000 and point[0] > -1000 \
                and point[1] < 1000 and point[1] > -1000 \
                and point[2] < 1000 and point[2] > -1000:
            return True
        else:
            return False

    def bytesarray_to_int16(self, bytes, start_idx):
        sign = bytes[start_idx + 1] & 0x80
        var = bytes[start_idx] | bytes[start_idx + 1] << 8
        val = struct.unpack('<i', struct.pack('I', var))[0]
        return (val - 65536) * (sign != 0) + (val) * (sign == 0)

    def bytesarray_to_int(self, bytes, start_idx):
        return bytes[start_idx] | bytes[start_idx + 1] << 8 | bytes[start_idx + 2] << 16 | bytes[start_idx + 3] << 24

    def bytesarray_to_floatarray(self, bytes):
        floatarray = []
        bytesize = len(bytes)
        i = 0
        while i < bytesize:
            var = bytes[i] | bytes[i + 1] << 8 | bytes[i + 2] << 16 | bytes[i + 3] << 24
            floatarray.append(struct.unpack('<f', struct.pack('I', var))[0])
            i += 4
        return np.array(floatarray, dtype = np.float64)  # 96 numbers

    def start(self, timeout):
        self.plotter_tmr(timeout)

    def add_node(name):
        Plotter.coordinates[name] = []

    def clustering(self, data, eps = 2, min_samples = 10):
        if len(data) < min_samples:
            return (0, len(data), 1.0)

        db = DBSCAN(eps = eps, min_samples = min_samples).fit(data)
        core_samples_mask = np.zeros_like(db.labels_, dtype = bool)
        core_samples_mask[db.core_sample_indices_] = True
        labels = db.labels_

        # Number of clusters in labels, ignoring noise if present.
        n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)
        n_noise_ = list(labels).count(-1)

        cluster = [np.zeros((1, 3)) for t in range(n_clusters_+ 1)]
        for i in range(len(data)):
            if(labels[i] == -1):
                cluster[labels[i]]=np.append(cluster[labels[i]],[data[i]],axis=0)
            else:
                cluster[labels[i]]=np.append(cluster[labels[i]],[data[i]],axis=0)

        return (n_clusters_, n_noise_, float(n_noise_ / len(data)))
