## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,er4f linker.cmd package/cfg/mmw_per4f.oer4f

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/mmw_per4f.xdl
	$(SED) 's"^\"\(package/cfg/mmw_per4fcfg.cmd\)\"$""\"D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss/Debug/configPkg/\1\""' package/cfg/mmw_per4f.xdl > $@
	-$(SETDATE) -r:max package/cfg/mmw_per4f.h compiler.opt compiler.opt.defs
