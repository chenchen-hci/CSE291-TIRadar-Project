import time as time
import multiprocessing
import sys


def set_proc_name(newname):
    from ctypes import cdll, byref, create_string_buffer
    libc = cdll.LoadLibrary('libc.so.6')
    buff = create_string_buffer(len(newname)+1)
    buff.value = newname
    libc.prctl(15, byref(buff), 0, 0, 0)

class Logger:
    def __init__(self, basename, node):
        self.node = node
        set_proc_name(bytes("python_logger_" + node, 'utf8'))
        self.f = open(basename + "_" + node + '.dat', 'a')

    def write(self, chunk):
        #chunk.append(0x20)
        if len(chunk) == 0:
            return

        if list(chunk)[-1] != str(' '):
            chunk.append(0x20)

        print("###############write data to ", self.node)
        s = " ".join(str(v) for v in list(chunk))
        self.f.write(s)
        self.f.flush()

    def run(self, queue):
        while True:
            buf = bytearray()
            while (not queue.empty()) and queue.qsize() < 100:
                print("child received ", queue.qsize())
                buf.extend(queue.get())
            self.write(buf)

    def __str__(self):
        return ""
