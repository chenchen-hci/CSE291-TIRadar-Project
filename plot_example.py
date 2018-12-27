from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import numpy as np

app = QtGui.QApplication([])
w = gl.GLViewWidget()

g = gl.GLGridItem()
a = gl.GLAxisItem()

w.addItem(g)
w.addItem(a)
w.show()

#generate random points from -10 to 10, z-axis positive
pos = np.random.randint(-10,10,size=(10,3))
pos[:,2] = np.abs(pos[:,2])
print(pos)
print(np.shape(pos))

sp2 = gl.GLScatterPlotItem(pos = pos)
w.addItem(sp2)

#generate a color opacity gradient
color = np.zeros((pos.shape[0],4), dtype=np.float32)
color[:,0] = 1
color[:,1] = 0
color[:,2] = 0.5
color[0:5,3] = np.arange(0,5)/5.
print(np.shape(color))   # 1000, 4

def update():
    ## update volume colors
#    global color

#    color = np.roll(color, 1, axis=0) # shift 1 along y axis

#    print(color)
 #   sp2.setData(color=color)
    a = np.array([[1,1,1], [2,2,2], [3,3,3], [4,4,4]])
    c = np.array([[1,1,1, 0.1], [2,2,2, 0.2], [3,3,3, 0.3], [4,4,4, 0.4]])
    sp2.setData(pos = a, color = (0.3,0.2, 0.7, 0.8))

t = QtCore.QTimer()
t.timeout.connect(update)
t.start(5)

## Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, PYQT_VERSION):
        QtGui.QApplication.instance().exec_()