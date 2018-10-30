################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ti-cgt-arm_16.9.6.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./HandCraftedclassifier.oer4f" \
"./config_edma_util.oer4f" \
"./config_hwa_util.oer4f" \
"./data_path.oer4f" \
"./gesture_swipe.oer4f" \
"./main_swipe.oer4f" \
"./mmw_cli.oer4f" \
"./post_processing.oer4f" \
"./rx_ch_bias_measure.oer4f" \
"./sensor_mgmt.oer4f" \
"../mss_gesture_swipe_linker.cmd" \
"../r4f_linker.cmd" \
$(GEN_CMDS__FLAG) \
-llibosal_xwr14xx.aer4f \
-llibesm_xwr14xx.aer4f \
-llibgpio_xwr14xx.aer4f \
-llibsoc_xwr14xx.aer4f \
-llibpinmux_xwr14xx.aer4f \
-llibcrc_xwr14xx.aer4f \
-llibuart_xwr14xx.aer4f \
-llibmailbox_xwr14xx.aer4f \
-llibmmwavelink_xwr14xx.aer4f \
-llibmmwave_xwr14xx.aer4f \
-llibadcbuf_xwr14xx.aer4f \
-llibdma_xwr14xx.aer4f \
-llibedma_xwr14xx.aer4f \
-llibcli_xwr14xx.aer4f \
-llibhwa_xwr14xx.aer4f \
-lrtsv7R4_T_le_v3D16_eabi.lib \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
xwr14xx_gesture_swipe_mss.xer4f \

EXE_OUTPUTS__QUOTED += \
"xwr14xx_gesture_swipe_mss.xer4f" \

BIN_OUTPUTS += \
xwr14xx_gesture_swipe_mss.hex \

BIN_OUTPUTS__QUOTED += \
"xwr14xx_gesture_swipe_mss.hex" \


# All Target
all:
	@$(MAKE) --no-print-directory pre-build
	@$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "xwr14xx_gesture_swipe_mss.xer4f"

# Tool invocations
xwr14xx_gesture_swipe_mss.xer4f: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f -z -m"xwr14xx_gesture_swipe_mss.map" --heap_size=0x800 --stack_size=0x800 -i"D:/ti/ti-cgt-arm_16.9.6.LTS/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/control/mmwave/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/control/mmwavelink/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/adcbuf/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/crc/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/dma/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/edma/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/esm/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/gpio/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/hwa/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/mailbox/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/osal/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/pinmux/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/soc/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/drivers/uart/lib" -i"D:/ti/mmwave_sdk_01_02_00_05/packages/ti/utils/cli/lib" -i"D:/ti/ti-cgt-arm_16.9.6.LTS/include" --reread_libs --disable_auto_rts --diag_warning=225 --diag_wrap=off --display_error_number --warn_sections --xml_link_info="xwr14xx_gesture_swipe_mss_linkInfo.xml" --rom_model --unused_section_elimination=on -o "xwr14xx_gesture_swipe_mss.xer4f" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

xwr14xx_gesture_swipe_mss.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armhex"  -o "xwr14xx_gesture_swipe_mss.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_CMDS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "HandCraftedclassifier.oer4f" "config_edma_util.oer4f" "config_hwa_util.oer4f" "data_path.oer4f" "gesture_swipe.oer4f" "main_swipe.oer4f" "mmw_cli.oer4f" "post_processing.oer4f" "rx_ch_bias_measure.oer4f" "sensor_mgmt.oer4f" 
	-$(RM) "HandCraftedclassifier.d" "config_edma_util.d" "config_hwa_util.d" "data_path.d" "gesture_swipe.d" "main_swipe.d" "mmw_cli.d" "post_processing.d" "rx_ch_bias_measure.d" "sensor_mgmt.d" 
	-@echo 'Finished clean'
	-@echo ' '

pre-build:
	-rm -f D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss/xwr14xx_gesture_swipe_mss.bin
	-@echo ' '

post-build:
	-D:/ti/mmwave_sdk_01_02_00_05/packages/scripts/ImageCreator/xwr14xx/out2rprc/out2rprc.exe xwr14xx_gesture_swipe_mss.xer4f xwr14xx_gesture_swipe_mss.bin 0x200000
	-D:/ti/mmwave_sdk_01_02_00_05/packages/scripts/ImageCreator/append_bin_crc/gen_bincrc32.exe xwr14xx_gesture_swipe_mss.bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

