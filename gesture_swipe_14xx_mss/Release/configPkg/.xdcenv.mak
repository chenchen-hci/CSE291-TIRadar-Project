#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/ti/bios_6_53_02_00/packages;D:/ti/ccsv7/ccs_base
override XDCROOT = D:/ti/xdctools_3_50_04_43_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/ti/bios_6_53_02_00/packages;D:/ti/ccsv7/ccs_base;D:/ti/xdctools_3_50_04_43_core/packages;..
HOSTOS = Windows
endif
