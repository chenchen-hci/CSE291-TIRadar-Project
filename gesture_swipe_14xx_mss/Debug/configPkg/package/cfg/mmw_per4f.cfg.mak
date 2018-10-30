# invoke SourceDir generated makefile for mmw.per4f
mmw.per4f: .libraries,mmw.per4f
.libraries,mmw.per4f: package/cfg/mmw_per4f.xdl
	$(MAKE) -f D:\mmwave_industrial_toolbox_2_5_2\labs\gesture_swipe_14xx_mss/src/makefile.libs

clean::
	$(MAKE) -f D:\mmwave_industrial_toolbox_2_5_2\labs\gesture_swipe_14xx_mss/src/makefile.libs clean

