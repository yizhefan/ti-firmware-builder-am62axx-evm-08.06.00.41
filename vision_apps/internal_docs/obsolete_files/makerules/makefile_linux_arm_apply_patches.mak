#
# Utility makefile to apply patches to linux components
#
#

PSDK_PATCHES_PATH=$(PSDK_PATH)/psdk_rtos_auto/patches/

LINUX_KERNEL_PATCHES =
LINUX_KERNEL_PATCHES += $(PSDK_PATCHES_PATH)/linux/0001-rpmsg-char-enhancements.patch
LINUX_KERNEL_PATCHES += $(PSDK_PATCHES_PATH)/linux/0002-Enabling-rpmsg_char-as-a-module-in-linux-kernel-buil.patch
LINUX_KERNEL_PATCHES += $(PSDK_PATCHES_PATH)/linux/0003-Adding-vision_apps-dtso-file-and-adding-entry-in-Mak.patch
LINUX_KERNEL_PATCHES += $(PSDK_PATCHES_PATH)/linux/0004-Updating-j721e-dtsi-file-for-memory-map-changes-in-v.patch
LINUX_KERNEL_PATCHES += $(PSDK_PATCHES_PATH)/linux/0005-Disabled-i2c6-in-vision_apps-dtso-to-allow-R5F-to-co.patch

LINUX_UBOOT_PATCHES =
LINUX_UBOOT_PATCHES += $(PSDK_PATCHES_PATH)/u-boot/0001-Updated-j721e-uboot-dts-file-for-memory-map-required.patch
LINUX_UBOOT_PATCHES += $(PSDK_PATCHES_PATH)/u-boot/0002-Updated-MMU-used-by-u-boot-for-j721e-based-on-memory.patch

LINUX_SYSFW_IMAGE_GEN_PATCHES =
LINUX_SYSFW_IMAGE_GEN_PATCHES += $(PSDK_PATCHES_PATH)/system-firmware-image-gen/0001-Modified-board-cfg-to-make-MSMC-cache-size-as-0-byte.patch
LINUX_SYSFW_IMAGE_GEN_PATCHES += $(PSDK_PATCHES_PATH)/system-firmware-image-gen/0002-Modified-rm-cfg-to-makr-all-non-linux-resources-as-H.patch
LINUX_SYSFW_IMAGE_GEN_PATCHES += $(PSDK_PATCHES_PATH)/system-firmware-image-gen/0003-Updated-rm-cfg.c-to-make-CSI2-VPAC-DMPAC-work-in-Lin.patch

linux_apply_patches_kernel:
	$(foreach patch_file, $(LINUX_KERNEL_PATCHES),\
		cd $(LINUX_KERNEL_PATH) && git am $(patch_file); \
	)

linux_apply_patches_uboot:
	$(foreach patch_file, $(LINUX_UBOOT_PATCHES),\
		cd $(LINUX_UBOOT_PATH) && git am $(patch_file); \
	)

linux_apply_patches_sysfw_image_gen:
	$(foreach patch_file, $(LINUX_SYSFW_IMAGE_GEN_PATCHES),\
		cd $(LINUX_SYSFW_IMAGE_GEN_PATH) && git am $(patch_file); \
	)

linux_apply_patches: linux_apply_patches_kernel linux_apply_patches_uboot linux_apply_patches_sysfw_image_gen
