#
# Utility makefile to build TIOVX, PTK and imaging libaries and related components
#
# Edit this file to suit your specific build needs
#

ifeq ($(PROFILE), $(filter $(PROFILE),release all))
LINUX_APP_PROFILE=release
endif
ifeq ($(PROFILE), $(filter $(PROFILE),debug all))
LINUX_APP_PROFILE=debug
endif

linux_fs_install:
ifeq ($(BUILD_CPU_MPU1),yes)
	# copy application binaries and scripts
	mkdir -p $(LINUX_FS_PATH)/opt/vision_apps
	cp $(VISION_APPS_PATH)/out/J7/A72/LINUX/$(LINUX_APP_PROFILE)/*.out $(LINUX_FS_PATH)/opt/vision_apps
	cp $(VISION_APPS_PATH)/apps/basic_demos/app_linux_fs_files/* $(LINUX_FS_PATH)/opt/vision_apps
	chmod +x $(LINUX_FS_PATH)/opt/vision_apps/*.sh
	# copy test data
	cp $(TIOVX_PATH)/conformance_tests/test_data/psdkra/app_opengl_mosaic/* $(LINUX_FS_PATH)/opt/vision_apps
endif

	# remove old remote firmware files from filesystem
	mkdir -p $(LINUX_FS_PATH)/lib/firmware
	-rm $(LINUX_FS_PATH)/lib/firmware/j7-*-fw -f

ifeq ($(BUILD_CPU_MCU2_0),yes)
	# copy remote firmware files for mcu2_0
	cp $(VISION_APPS_PATH)/out/J7/R5F/SYSBIOS/$(LINUX_APP_PROFILE)/vx_app_tirtos_linux_mcu2_0.out $(LINUX_FS_PATH)/lib/firmware/j7-main-r5f0_0-fw
endif
ifeq ($(BUILD_CPU_MCU2_1),yes)
	# copy remote firmware files for mcu2_1
	cp $(VISION_APPS_PATH)/out/J7/R5F/SYSBIOS/$(LINUX_APP_PROFILE)/vx_app_tirtos_linux_mcu2_1.out $(LINUX_FS_PATH)/lib/firmware/j7-main-r5f0_1-fw
endif
ifeq ($(BUILD_CPU_C6x_1),yes)
	# copy remote firmware files for c6x_1
	cp $(VISION_APPS_PATH)/out/J7/C66/SYSBIOS/$(LINUX_APP_PROFILE)/vx_app_tirtos_linux_c6x_1.out $(LINUX_FS_PATH)/lib/firmware/j7-c66_0-fw
endif
ifeq ($(BUILD_CPU_C6x_2),yes)
	# copy remote firmware files for c6x_2
	cp $(VISION_APPS_PATH)/out/J7/C66/SYSBIOS/$(LINUX_APP_PROFILE)/vx_app_tirtos_linux_c6x_2.out $(LINUX_FS_PATH)/lib/firmware/j7-c66_1-fw
endif
ifeq ($(BUILD_CPU_C7x_1),yes)
	# copy remote firmware files for c7x_1
	cp $(VISION_APPS_PATH)/out/J7/C71/SYSBIOS/$(LINUX_APP_PROFILE)/vx_app_tirtos_linux_c7x_1.out $(LINUX_FS_PATH)/lib/firmware/j7-c71_0-fw
endif
	# copy uboot files
	cp $(KS3_LINUX_INTEGRATED_PATH)/u-boot/arm64/tispl.bin $(LINUX_FS_PATH)/boot/
	cp $(KS3_LINUX_INTEGRATED_PATH)/u-boot/arm64/u-boot.img $(LINUX_FS_PATH)/boot/
	cp $(KS3_LINUX_INTEGRATED_PATH)/u-boot/r5/tiboot3.bin $(LINUX_FS_PATH)/boot/
	cp $(KS3_LINUX_INTEGRATED_PATH)/system-firmware-image-gen/sysfw.itb $(LINUX_FS_PATH)/boot/
	# copy linux kernel related files
	cp $(KS3_LINUX_INTEGRATED_PATH)/linux/arch/arm64/boot/Image $(LINUX_FS_PATH)/boot/
	cp $(KS3_LINUX_INTEGRATED_PATH)/linux/arch/arm64/boot/dts/ti/*.dtb $(LINUX_FS_PATH)/boot/
	cp $(KS3_LINUX_INTEGRATED_PATH)/linux/arch/arm64/boot/dts/ti/*.dtbo $(LINUX_FS_PATH)/boot/
	# copy to $(LINUX_FS_PATH) done !!!

linux_fs_install_sd: linux_fs_install
	# copy vision apps binaries
	mkdir -p $(LINUX_SD_FS_ROOT_PATH)/opt/vision_apps
	cp -r $(LINUX_FS_PATH)/opt/vision_apps $(LINUX_SD_FS_ROOT_PATH)/opt/

	# copy remote core firmware's
	mkdir -p $(LINUX_SD_FS_ROOT_PATH)/lib/firmware
	-rm $(LINUX_SD_FS_ROOT_PATH)/lib/firmware/j7-*-fw -f
	cp $(LINUX_FS_PATH)/lib/firmware/j7-*-fw $(LINUX_SD_FS_ROOT_PATH)/lib/firmware/

	# copy uboot related files
	cp $(LINUX_FS_PATH)/boot/tispl.bin $(LINUX_SD_FS_BOOT_PATH)/
	cp $(LINUX_FS_PATH)/boot/u-boot.img $(LINUX_SD_FS_BOOT_PATH)/
	cp $(LINUX_FS_PATH)/boot/tiboot3.bin $(LINUX_SD_FS_BOOT_PATH)/
	cp $(LINUX_FS_PATH)/boot/sysfw.itb $(LINUX_SD_FS_BOOT_PATH)/
	cp $(VISION_APPS_PATH)/apps/basic_demos/app_linux_fs_files/uEnv.txt $(LINUX_SD_FS_BOOT_PATH)/

	# copy linux kernel related files
	cp $(LINUX_FS_PATH)/boot/Image $(LINUX_SD_FS_ROOT_PATH)/boot/
	cp $(LINUX_FS_PATH)/boot/k3-j721e-common-proc-board.dtb $(LINUX_SD_FS_ROOT_PATH)/boot/
	cp $(LINUX_FS_PATH)/boot/k3-j721e-vision_apps.dtbo $(LINUX_SD_FS_ROOT_PATH)/boot/

	# sync'ing
	sync
	# copy to $(LINUX_SD_FS_ROOT_PATH), $(LINUX_SD_FS_BOOT_PATH) done !!!

linux_fs_install_sd_modules:
	cp -r $(LINUX_FS_PATH)/lib/modules/* $(LINUX_SD_FS_ROOT_PATH)/lib/modules
	sync

linux_fs_install_sd_all:
	cp -r $(LINUX_FS_PATH)/* $(LINUX_SD_FS_ROOT_PATH)/
	sync

linux_kernel_modules:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(LINUX_KERNEL_PATH) modules
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(LINUX_KERNEL_PATH) INSTALL_MOD_PATH=$(LINUX_FS_PATH) modules_install

linux_kernel:
	cd $(KS3_LINUX_INTEGRATED_PATH)/ && make linux

linux_uboot:
	cd $(KS3_LINUX_INTEGRATED_PATH)/ && make u-boot

linux_sysfw:
	cd $(KS3_LINUX_INTEGRATED_PATH)/ && make sysfw

