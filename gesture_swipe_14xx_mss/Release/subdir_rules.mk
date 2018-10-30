################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
HandCraftedclassifier.oer4f: ../HandCraftedclassifier.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="HandCraftedclassifier.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

config_edma_util.oer4f: ../config_edma_util.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="config_edma_util.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

config_hwa_util.oer4f: ../config_hwa_util.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="config_hwa_util.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

data_path.oer4f: ../data_path.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="data_path.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

gesture_swipe.oer4f: ../gesture_swipe.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="gesture_swipe.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main_swipe.oer4f: ../main_swipe.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="main_swipe.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-2008108255:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-2008108255-inproc

build-2008108255-inproc: ../mmw.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"D:/ti/xdctools_3_50_04_43_core/xs" --xdcpath="D:/ti/bios_6_53_02_00/packages;D:/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.R4F -p ti.platforms.cortexR:IWR14XX:false:200 -r release -c "D:/ti/ti-cgt-arm_16.9.6.LTS" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-2008108255 ../mmw.cfg
configPkg/compiler.opt: build-2008108255
configPkg/: build-2008108255

mmw_cli.oer4f: ../mmw_cli.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="mmw_cli.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

post_processing.oer4f: ../post_processing.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="post_processing.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

rx_ch_bias_measure.oer4f: ../rx_ch_bias_measure.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="rx_ch_bias_measure.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sensor_mgmt.oer4f: ../sensor_mgmt.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="D:/mmwave_industrial_toolbox_2_5_2/labs/gesture_swipe_14xx_mss" --include_path="D:/ti/mmwave_sdk_01_02_00_05/packages" --include_path="D:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR14XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=MMWAVE_L3RAM_SIZE=0x40000 --define=DebugP_ASSERT_ENABLED -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=packed --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="sensor_mgmt.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


