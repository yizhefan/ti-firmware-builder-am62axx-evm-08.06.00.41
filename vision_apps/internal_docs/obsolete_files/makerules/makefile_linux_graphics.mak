#
# Utility makefile to build DDK
#
# Edit this file to suit your specific build needs
#

linux_gfx_ddk: linux_gfx_ddk_kernel_install

linux_gfx_ddk_kernel_install:
	sudo $(VISION_APPS_PATH)/tools/scripts/build-rgx.sh -k $(LINUX_KERNEL_OBJECT_PATH) --cc-root $(GCC_LINUX_ARM_ROOT) --cc aarch64-linux-gnu- --ws nulldrmws --fs $(LINUX_FS_PATH) --install -- $(LINUX_DDK_PATH)

linux_gfx_ddk_clean:
	sudo $(VISION_APPS_PATH)/build-rgx.sh --clean -- $(LINUX_DDK_PATH)
