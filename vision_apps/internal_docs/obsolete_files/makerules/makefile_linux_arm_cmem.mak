#
# Utility makefile to build TIOVX, PTK and imaging libaries and related components
#
# Edit this file to suit your specific build needs
#

export CC=$(GCC_LINUX_ARM_ROOT)/bin/aarch64-linux-gnu-gcc 
export TOOLCHAIN_PREFIX=$(GCC_LINUX_ARM_ROOT)/bin/aarch64-linux-gnu-

cmem: cmem_kernel cmem_user cmem_install

cmem_kernel: cmem_configure
	make -C $(LINUX_CMEM_PATH)/src/cmem/module/ KERNEL_INSTALL_DIR=$(LINUX_KERNEL_OBJECT_PATH) ARCH=arm64 TOOLCHAIN_PREFIX=$(GCC_LINUX_ARM_ROOT)/bin/aarch64-linux-gnu- release 

cmem_user: cmem_configure
	make -C $(LINUX_CMEM_PATH)/src/cmem/api/  

cmem_configure:
	cd $(LINUX_CMEM_PATH) && ./configure --enable-shared --host=arm-linux-gnueabihf --prefix="/usr"

cmem_install:
	mkdir -p $(LINUX_FS_PATH)/opt/vision_apps
	cp $(LINUX_CMEM_PATH)/src/cmem/module/*.ko $(LINUX_FS_PATH)/opt/vision_apps
	cp $(LINUX_CMEM_PATH)/src/cmem/api/.libs/* $(LINUX_FS_PATH)/usr/lib
    
cmem_clean:
	make -C $(LINUX_CMEM_PATH)/src/cmem/api/ clean
	make -C $(LINUX_CMEM_PATH)/src/cmem/module/ KERNEL_INSTALL_DIR=$(LINUX_KERNEL_PATH) ARCH=arm64 TOOLCHAIN_PREFIX=$(GCC_LINUX_ARM_ROOT)/bin/aarch64-linux-gnu- clean
	make -C $(LINUX_CMEM_PATH)/src/cmem/tests/ clean

